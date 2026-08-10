#include "../include/bfs.h"
#include <queue>

// Breadth-First Search (BFS) implementation on a CSR representation.
// Uses a queue to explore vertices layer-by-layer and calculates edge-count distances.
BFSResult bfs(const CSRGraph& struct_graph, int i_source)
{
    // The number of vertices is derived from row_ptr size minus 1.
    int i_vertex_count = static_cast<int>(struct_graph.v_row_ptr.size()) - 1;

    BFSResult struct_result;
    struct_result.v_traversal.reserve(i_vertex_count);
    struct_result.v_distances.assign(i_vertex_count, -1);

    // Track visited states to prevent cycles/redundancy.
    std::vector<bool> v_visited(i_vertex_count, false);
    std::queue<int> q_vertices;

    // Initialize search with source node.
    v_visited[i_source] = true;
    struct_result.v_distances[i_source] = 0;
    q_vertices.push(i_source);

    while (!q_vertices.empty())
    {
        // Dequeue next vertex to process.
        int i_current = q_vertices.front();
        q_vertices.pop();
        struct_result.v_traversal.push_back(i_current);

        // Fetch boundaries of the adjacency list inside row_ptr array.
        int i_edge_start = struct_graph.v_row_ptr[i_current];
        int i_edge_end = struct_graph.v_row_ptr[i_current + 1];

        // Iterate through all neighbors of the current vertex.
        for (int i_idx = i_edge_start; i_idx < i_edge_end; ++i_idx)
        {
            int i_neighbour = struct_graph.v_col_index[i_idx];

            // If neighbor is unvisited, mark it and enqueue it.
            if (!v_visited[i_neighbour])
            {
                v_visited[i_neighbour] = true;
                struct_result.v_distances[i_neighbour] = struct_result.v_distances[i_current] + 1;
                q_vertices.push(i_neighbour);
            }
        }
    }

    return struct_result;
}
