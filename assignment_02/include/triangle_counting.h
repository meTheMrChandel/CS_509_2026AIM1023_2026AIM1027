#ifndef TRIANGLE_COUNTING_H
#define TRIANGLE_COUNTING_H

#include "graph.h"
#include <vector>

struct Triangle {
    int u, v, w;
};

struct TriangleResult {
    long long total_triangles;
    std::vector<Triangle> triangles_found;
};

// Computes the total number of triangles in an undirected graph.
// Finds individual triangles for small graphs (V <= 100).
TriangleResult count_triangles(const AdjList& adj_list);

#endif // TRIANGLE_COUNTING_H
