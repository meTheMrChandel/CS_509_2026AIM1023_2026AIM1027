#include "../include/triangle_counting.h"
#include <algorithm>

static bool has_edge(const std::vector<Edge>& neighbors, int dest) {
    auto it = std::lower_bound(neighbors.begin(), neighbors.end(), dest,
        [](const Edge& edge, int target) {
            return edge.i_destination < target;
        });
    return (it != neighbors.end() && it->i_destination == dest);
}

TriangleResult count_triangles(const AdjList& adj_list) {
    TriangleResult result;
    result.total_triangles = 0;

    int n = static_cast<int>(adj_list.size());

    // Iterate u < v < w
    for (int u = 0; u < n; ++u) {
        const auto& u_neighbors = adj_list[u];
        int u_deg = static_cast<int>(u_neighbors.size());

        for (int i = 0; i < u_deg; ++i) {
            int v = u_neighbors[i].i_destination;
            if (v <= u) continue;

            for (int j = i + 1; j < u_deg; ++j) {
                int w = u_neighbors[j].i_destination;
                if (w <= v) continue;

                // Check if there is an edge between v and w
                if (has_edge(adj_list[v], w)) {
                    result.total_triangles++;
                    if (n <= 100) {
                        result.triangles_found.push_back({u, v, w});
                    }
                }
            }
        }
    }

    return result;
}
