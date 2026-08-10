#ifndef DFS_H
#define DFS_H

#include "csr.h"
#include <vector>

// Struct storing outcomes of Depth-First Search traversal.
struct DFSResult
{
    std::vector<int> v_traversal; // List of vertices in order visited.
};

// Computes DFS traversal over a CSR Graph starting from i_source.
DFSResult dfs(const CSRGraph& struct_graph, int i_source);

#endif // DFS_H
