#include "../include/csr.h"

void convert_to_csr(const AdjList& adj, std::vector<int>& row_ptr, std::vector<int>& col_idx) {
    int V = static_cast<int>(adj.size());
    row_ptr.resize(V + 1);
    col_idx.clear();

    int edge_count = 0;
    for (int i = 0; i < V; ++i) {
        row_ptr[i] = edge_count;
        for (int neighbor : adj[i]) {
            col_idx.push_back(neighbor);
            edge_count++;
        }
    }
    row_ptr[V] = edge_count;
}
