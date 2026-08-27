#ifndef CSR_H
#define CSR_H

#include "graph.h"
#include <vector>

// Converts an Adjacency List to separate CSR vectors (row_ptr and col_idx)
void convert_to_csr(const AdjList& adj, std::vector<int>& row_ptr, std::vector<int>& col_idx);

#endif // CSR_H
