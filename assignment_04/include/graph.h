#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>

using AdjList = std::vector<std::vector<int>>;

struct PageRankParams {
    double damping = 0.85;
    double tolerance = 0.0001;
    int max_iterations = 100;
};

// Reads and validates an undirected, unweighted graph for Vertex Coloring.
// If invalid, prints an error and returns false.
bool read_vertex_coloring_graph(const std::string& filename, int& V, int& E, AdjList& adj);

// Reads and validates a directed, unweighted graph and parameters for PageRank.
// If invalid, prints an error and returns false.
bool read_pagerank_graph(const std::string& filename, int& V, int& E, AdjList& adj, PageRankParams& params);

#endif // GRAPH_H
