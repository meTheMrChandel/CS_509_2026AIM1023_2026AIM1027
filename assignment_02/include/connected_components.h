#ifndef CONNECTED_COMPONENTS_H
#define CONNECTED_COMPONENTS_H

#include "graph.h"
#include <vector>

struct CCResult {
    int total_components;
    std::vector<int> v_components; // Component ID for each vertex.
};

// Computes the connected components on an undirected graph using BFS
CCResult find_connected_components(const AdjList& adj_list);

#endif // CONNECTED_COMPONENTS_H
