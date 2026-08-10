#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>

// Struct representing a graph edge.
struct Edge
{
    int i_destination; // Destination vertex ID.
    int i_weight;      // Positive edge weight.
};

// Type alias representing adjacency list structure.
using AdjList = std::vector<std::vector<Edge>>;

// Struct container holding all read parameters of the graph.
struct GraphInput
{
    AdjList v_adj_list;     // Adjacency list representation.
    int i_vertex_count;     // Total vertex count (V).
    int i_edge_count;       // Total edge count (E).
    int i_source;           // Specified source vertex for traversals.
};

// Reads an unweighted graph file and builds the adjacency list.
bool read_unweighted_graph(const std::string& str_filename, GraphInput& struct_input);

// Reads a weighted graph file and builds the adjacency list.
bool read_weighted_graph(const std::string& str_filename, GraphInput& struct_input);

#endif // GRAPH_H
