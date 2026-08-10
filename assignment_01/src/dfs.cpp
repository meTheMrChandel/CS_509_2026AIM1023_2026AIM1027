#include "../include/dfs.h"
#include <vector>

// Depth-First Search (DFS) traversal implementation on a CSR representation.
// Uses an explicit stack on the heap instead of recursive function calls.
// Note: On Windows systems, thread stacks are limited to 1MB or 2MB by default.
// Recursive DFS on large graphs (e.g. V=50,000 or V=100,000) causes stack overflow crashes.
// Implementing an iterative DFS with a heap-allocated std::vector stack bypasses this limitation.
DFSResult dfs(const CSRGraph& struct_graph, int i_source)
{
    int i_vertex_count = static_cast<int>(struct_graph.v_row_ptr.size()) - 1;

    DFSResult struct_result;
    struct_result.v_traversal.reserve(i_vertex_count);

    // Track visited nodes to avoid cyclic loops.
    std::vector<bool> v_visited(i_vertex_count, false);
    
    // Explicit stack representing active vertices in the traversal.
    std::vector<int> v_stack;

    // Start with the source vertex.
    v_stack.push_back(i_source);

    while (!v_stack.empty())
    {
        int i_current = v_stack.back();
        v_stack.pop_back();

        // Nodes may be pushed onto the stack multiple times before being visited.
        // Process only if it has not been visited yet.
        if (!v_visited[i_current])
        {
            v_visited[i_current] = true;
            struct_result.v_traversal.push_back(i_current);

            int i_edge_start = struct_graph.v_row_ptr[i_current];
            int i_edge_end = struct_graph.v_row_ptr[i_current + 1];

            // Push neighbor connections onto the stack.
            // Loop in reverse order (from end to start) so that the first neighbor
            // in the adjacency list is processed first (since stacks are Last-In-First-Out).
            for (int i_idx = i_edge_end - 1; i_idx >= i_edge_start; --i_idx)
            {
                int i_neighbour = struct_graph.v_col_index[i_idx];
                if (!v_visited[i_neighbour])
                {
                    v_stack.push_back(i_neighbour);
                }
            }
        }
    }

    return struct_result;
}
