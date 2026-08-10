#include "../include/csr.h"

// Compressed Sparse Row (CSR) converter implementation.
// Flattens adjacency lists into three contiguous vectors for cache efficiency.
CSRGraph convert_to_csr(const AdjList& v_graph)
{
    CSRGraph struct_csr;

    int i_vertex_count = static_cast<int>(v_graph.size());

    // Allocate row pointer structure, size is V + 1.
    struct_csr.v_row_ptr.resize(i_vertex_count + 1);

    int i_edge_idx = 0;

    // Process adjacency vectors for each vertex.
    for (int i_v = 0; i_v < i_vertex_count; ++i_v)
    {
        // Store current edge offset for this vertex.
        struct_csr.v_row_ptr[i_v] = i_edge_idx;

        // Traverse all edges associated with vertex i_v.
        for (const Edge& struct_edge : v_graph[i_v])
        {
            // Record target column index and weight values.
            struct_csr.v_col_index.push_back(struct_edge.i_destination);
            struct_csr.v_weights.push_back(struct_edge.i_weight);
            i_edge_idx++;
        }
    }

    // Set trailing row offset equal to total edges count (E).
    struct_csr.v_row_ptr[i_vertex_count] = i_edge_idx;

    return struct_csr;
}
