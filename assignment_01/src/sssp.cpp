#include "../include/sssp.h"
#include <limits>
#include <queue>

// Dijkstra's Single-Source Shortest Path (SSSP) algorithm implementation.
// Finds the shortest paths from i_source to all other nodes in a positive-weighted graph.
SSSPResult dijkstra(const CSRGraph& struct_graph, int i_source)
{
    int i_vertex_count = static_cast<int>(struct_graph.v_row_ptr.size()) - 1;

    SSSPResult struct_result;
    // Initialize distances to infinity (represented by max integer).
    struct_result.v_distances.assign(i_vertex_count, std::numeric_limits<int>::max());

    // Priority queue of pairs: <distance, vertex_id>.
    // Uses std::greater to extract the node with the minimum distance first.
    using NodePair = std::pair<int, int>;
    std::priority_queue<NodePair, std::vector<NodePair>, std::greater<NodePair>> pq_nodes;

    // Set source node distance to 0 and push it to the priority queue.
    struct_result.v_distances[i_source] = 0;
    pq_nodes.push({0, i_source});

    while (!pq_nodes.empty())
    {
        // Extract the vertex with the minimum distance.
        int i_curr_dist = pq_nodes.top().first;
        int i_curr_vertex = pq_nodes.top().second;
        pq_nodes.pop();

        // If a shorter path to this vertex has already been found, skip processing.
        if (i_curr_dist > struct_result.v_distances[i_curr_vertex])
        {
            continue;
        }

        int i_edge_start = struct_graph.v_row_ptr[i_curr_vertex];
        int i_edge_end = struct_graph.v_row_ptr[i_curr_vertex + 1];

        // Traverse neighbors of the current vertex.
        for (int i_idx = i_edge_start; i_idx < i_edge_end; ++i_idx)
        {
            int i_neighbour = struct_graph.v_col_index[i_idx];
            int i_weight = struct_graph.v_weights[i_idx];

            // Relax the edge if a shorter path to the neighbor is found.
            if (struct_result.v_distances[i_curr_vertex] + i_weight < struct_result.v_distances[i_neighbour])
            {
                struct_result.v_distances[i_neighbour] = struct_result.v_distances[i_curr_vertex] + i_weight;
                pq_nodes.push({struct_result.v_distances[i_neighbour], i_neighbour});
            }
        }
    }

    return struct_result;
}
