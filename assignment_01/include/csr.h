#ifndef CSR_H
#define CSR_H

#include "graph.h"
#include <vector>

// Struct representing the Compressed Sparse Row (CSR) format graph.
struct CSRGraph
{
    std::vector<int> v_row_ptr;    // Row offsets indicating starts/ends of vertex edges. Size: V + 1.
    std::vector<int> v_col_index;  // Destinational column vertex indices. Size: E (directed) / 2E (undirected).
    std::vector<int> v_weights;    // Edge weights associated with each column index. Size: E / 2E.
};

// Converts standard Adjacency List to CSR representation.
CSRGraph convert_to_csr(const AdjList& v_graph);

#endif // CSR_H
