#include "../include/graph_coloring.h"
#include "../include/timer.h"
#include <algorithm>
#include <vector>

struct VertexDegree {
    int id;
    int degree;
};

ColoringResult run_greedy_vertex_coloring(int V, const std::vector<int>& row_ptr, const std::vector<int>& col_idx) {
    ColoringResult result;
    result.colors.assign(V, -1);

    // Start timing only the algorithm execution time
    TimePoint start = start_timer();

    // 1. Compute degrees
    std::vector<VertexDegree> vertices(V);
    for (int i = 0; i < V; ++i) {
        vertices[i].id = i;
        vertices[i].degree = row_ptr[i + 1] - row_ptr[i];
    }

    // 2. Sort by degree (descending), then ID (ascending) to ensure stability and deterministic ordering
    std::sort(vertices.begin(), vertices.end(), [](const VertexDegree& a, const VertexDegree& b) {
        if (a.degree != b.degree) {
            return a.degree > b.degree;
        }
        return a.id < b.id;
    });

    // 3. Greedy coloring loop
    std::vector<bool> unavailable;

    int max_color_assigned = -1;
    for (int i = 0; i < V; ++i) {
        int u = vertices[i].id;

        int start_idx = row_ptr[u];
        int end_idx = row_ptr[u + 1];

        // Mark neighbor colors as unavailable
        for (int idx = start_idx; idx < end_idx; ++idx) {
            int neighbor = col_idx[idx];
            int n_color = result.colors[neighbor];
            if (n_color != -1) {
                if (n_color >= static_cast<int>(unavailable.size())) {
                    unavailable.resize(n_color + 1, false);
                }
                unavailable[n_color] = true;
            }
        }

        // Find the smallest color index not used by neighbors
        int assigned_color = 0;
        while (assigned_color < static_cast<int>(unavailable.size()) && unavailable[assigned_color]) {
            assigned_color++;
        }

        result.colors[u] = assigned_color;
        if (assigned_color > max_color_assigned) {
            max_color_assigned = assigned_color;
        }

        // Reset unavailable array flags back to false for the next vertex
        for (int idx = start_idx; idx < end_idx; ++idx) {
            int neighbor = col_idx[idx];
            int n_color = result.colors[neighbor];
            if (n_color != -1 && n_color < static_cast<int>(unavailable.size())) {
                unavailable[n_color] = false;
            }
        }
    }

    result.colors_used = (V == 0) ? 0 : (max_color_assigned + 1);
    result.elapsed_time_ms = stop_timer(start);

    // 4. Verify correctness of the coloring (post-algorithm verification, not timed)
    bool valid = true;
    for (int u = 0; u < V; ++u) {
        int start_idx = row_ptr[u];
        int end_idx = row_ptr[u + 1];
        for (int idx = start_idx; idx < end_idx; ++idx) {
            int neighbor = col_idx[idx];
            if (result.colors[u] == result.colors[neighbor]) {
                valid = false;
                break;
            }
        }
        if (!valid) break;
    }
    result.is_valid = valid;

    return result;
}
