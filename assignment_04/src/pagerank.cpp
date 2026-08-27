#include "../include/pagerank.h"
#include "../include/timer.h"
#include <cmath>
#include <vector>

PageRankResult run_pagerank(int V, const std::vector<int>& row_ptr, const std::vector<int>& col_idx, const PageRankParams& params) {
    PageRankResult result;
    if (V <= 0) {
        result.converged = true;
        return result;
    }

    result.ranks.assign(V, 1.0 / V);

    double d = params.damping;
    double tol = params.tolerance;
    int max_iter = params.max_iterations;

    // Start timing only the algorithm execution time (the update loop)
    TimePoint start = start_timer();

    std::vector<double> prev_ranks(V);
    std::vector<int> out_deg(V);
    for (int i = 0; i < V; ++i) {
        out_deg[i] = row_ptr[i + 1] - row_ptr[i];
    }

    int iter = 0;
    bool converged = false;

    while (iter < max_iter) {
        prev_ranks = result.ranks;
        std::vector<double> next_ranks(V, 0.0);
        double dangling_sum = 0.0;

        for (int u = 0; u < V; ++u) {
            if (out_deg[u] == 0) {
                dangling_sum += prev_ranks[u];
            } else {
                double contrib = prev_ranks[u] / out_deg[u];
                int start_idx = row_ptr[u];
                int end_idx = row_ptr[u + 1];
                for (int idx = start_idx; idx < end_idx; ++idx) {
                    int v = col_idx[idx];
                    next_ranks[v] += contrib;
                }
            }
        }

        // Apply damping and dangling node redistribution
        double base_value = (1.0 - d) / V;
        double dangling_contrib = d * (dangling_sum / V);
        double total_diff = 0.0;

        for (int v = 0; v < V; ++v) {
            next_ranks[v] = base_value + d * next_ranks[v] + dangling_contrib;
            total_diff += std::abs(next_ranks[v] - prev_ranks[v]);
        }

        result.ranks = next_ranks;
        iter++;

        if (total_diff <= tol) {
            converged = true;
            break;
        }
    }

    result.elapsed_time_ms = stop_timer(start);
    result.iterations = iter;
    result.converged = converged;

    return result;
}
