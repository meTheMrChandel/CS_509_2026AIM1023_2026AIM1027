#ifndef BETWEENNESS_CENTRALITY_H
#define BETWEENNESS_CENTRALITY_H

#include "graph.h"
#include <vector>

struct BCResult {
    std::vector<double> v_centrality;
};

// Computes vertex betweenness centrality using Brandes' algorithm.
// Supports both unweighted and non-negative weighted undirected graphs.
BCResult compute_betweenness_centrality(const AdjList& adj_list);

#endif // BETWEENNESS_CENTRALITY_H
