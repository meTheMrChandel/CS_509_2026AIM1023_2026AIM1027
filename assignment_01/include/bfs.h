#ifndef BFS_H
#define BFS_H

#include "csr.h"
#include <vector>

// Struct storing outcomes of Breadth-First Search traversal.
struct BFSResult
{
    std::vector<int> v_traversal; // List of vertices in order visited.
    std::vector<int> v_distances; // Unweighted shortest distance from source to each vertex.
};

// Computes BFS over a CSR Graph starting from i_source.
BFSResult bfs(const CSRGraph& struct_graph, int i_source);

#endif // BFS_H
