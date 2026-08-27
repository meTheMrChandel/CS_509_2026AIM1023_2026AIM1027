#ifndef GRAPH_COLORING_H
#define GRAPH_COLORING_H

#include <vector>

struct ColoringResult {
    std::vector<int> colors; // Color assigned to each vertex. Size: V.
    int colors_used = 0;
    bool is_valid = false;
    double elapsed_time_ms = 0.0;
};

ColoringResult run_greedy_vertex_coloring(int V, const std::vector<int>& row_ptr, const std::vector<int>& col_idx);

#endif // GRAPH_COLORING_H
