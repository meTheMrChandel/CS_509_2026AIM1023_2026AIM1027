#ifndef PAGERANK_H
#define PAGERANK_H

#include "graph.h"
#include <vector>

struct PageRankResult {
    std::vector<double> ranks;
    int iterations = 0;
    bool converged = false;
    double elapsed_time_ms = 0.0;
};

PageRankResult run_pagerank(int V, const std::vector<int>& row_ptr, const std::vector<int>& col_idx, const PageRankParams& params);

#endif // PAGERANK_H
