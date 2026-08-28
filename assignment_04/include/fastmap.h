#ifndef FASTMAP_H
#define FASTMAP_H

#include <vector>
#include <string>
#include <utility>

struct FastMapParams {
    int N = 0;
    int K = 0; // target embedding dimensionality
};

struct FastMapResult {
    std::vector<std::vector<double>> coordinates; // N x K final coordinates
    std::vector<std::pair<int, int>> pivots;       // pivot pair chosen for each of the K dimensions
    double avg_distance_error = 0.0;               // avg |embedded - original| pairwise distance
    double elapsed_time_ms = 0.0;
};

// Reads an N x N pairwise distance matrix and the target dimensionality K
// from a file. Validates: square/symmetric matrix, zero diagonal,
// non-negative distances, and 0 < K < N.
bool read_fastmap_input(const std::string& filename, std::vector<std::vector<double>>& dist, FastMapParams& params);

// Runs the FastMap heuristic on the provided distance matrix.
FastMapResult run_fastmap(const std::vector<std::vector<double>>& dist, const FastMapParams& params);

#endif // FASTMAP_H
