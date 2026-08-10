#ifndef SSSP_H
#define SSSP_H

#include "csr.h"
#include <vector>

// Struct storing outcomes of Dijkstra Single-Source Shortest Path.
struct SSSPResult
{
    std::vector<int> v_distances; // Shortest path distance from source to each vertex.
};

// Computes Dijkstra's SSSP over a CSR Graph starting from i_source.
SSSPResult dijkstra(const CSRGraph& struct_graph, int i_source);

#endif // SSSP_H
