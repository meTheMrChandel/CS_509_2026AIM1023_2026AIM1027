#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>

struct Edge {
    int i_destination;
    int i_weight;
};

using AdjList = std::vector<std::vector<Edge>>;

struct GraphInput {
    AdjList v_adj_list;
    int i_vertex_count;
    int i_edge_count;
};

// Reads undirected graph from file and populates the AdjList.
// Supports both weighted and unweighted, and rejects negative weights.
// Also sorts neighbors of each vertex by destination ID.
bool read_undirected_graph(const std::string& str_filename, GraphInput& struct_input);

#endif // GRAPH_H
