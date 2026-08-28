#include "../include/fastmap.h"
#include "../include/timer.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>
#include <random>
#include <algorithm>

static std::ifstream open_file(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::string alt = "tests/" + filename;
        fin.open(alt);
    }
    return fin;
}

bool read_fastmap_input(const std::string& filename, std::vector<std::vector<double>>& dist, FastMapParams& params) {
    std::ifstream fin = open_file(filename);
    if (!fin.is_open()) {
        std::cerr << "Error: Unable to open file '" << filename << "'\n";
        return false;
    }

    if (!(fin >> params.N >> params.K)) {
        std::cerr << "Error: Failed to read N and K.\n";
        return false;
    }

    if (params.N <= 0) {
        std::cerr << "Error: Invalid number of objects (N = " << params.N << "). Must be positive.\n";
        return false;
    }
    if (params.K <= 0 || params.K >= params.N) {
        std::cerr << "Error: Target dimensionality K (" << params.K
                  << ") must be positive and less than N (" << params.N << ").\n";
        return false;
    }

    dist.assign(params.N, std::vector<double>(params.N));
    for (int i = 0; i < params.N; ++i) {
        for (int j = 0; j < params.N; ++j) {
            if (!(fin >> dist[i][j])) {
                std::cerr << "Error: Failed to read distance value at row " << i
                          << ", column " << j << ".\n";
                return false;
            }
        }
    }

    const double EPS = 1e-6;
    for (int i = 0; i < params.N; ++i) {
        if (std::fabs(dist[i][i]) > EPS) {
            std::cerr << "Error: Non-zero diagonal entry at (" << i << ", " << i
                      << ") = " << dist[i][i] << ".\n";
            return false;
        }
        for (int j = 0; j < params.N; ++j) {
            if (dist[i][j] < 0.0) {
                std::cerr << "Error: Negative distance at (" << i << ", " << j
                          << ") = " << dist[i][j] << ".\n";
                return false;
            }
            if (std::fabs(dist[i][j] - dist[j][i]) > 1e-6) {
                std::cerr << "Error: Distance matrix is not symmetric at (" << i
                          << ", " << j << "): " << dist[i][j] << " vs " << dist[j][i] << ".\n";
                return false;
            }
        }
    }

    fin.close();
    return true;
}

// Finds the object farthest from 'from' under the current (possibly
// deflated) distance matrix.
static int farthest_from(int from, const std::vector<std::vector<double>>& d, int N) {
    int best = from;
    double best_dist = -1.0;
    for (int i = 0; i < N; ++i) {
        if (i == from) continue;
        if (d[from][i] > best_dist) {
            best_dist = d[from][i];
            best = i;
        }
    }
    return best;
}

// Fast heuristic pivot selection (Section 2.4): pick a random object, find
// the object farthest from it, then find the object farthest from that
// one; repeat the process a couple of times to improve the pivot choice.
static std::pair<int, int> choose_pivots(const std::vector<std::vector<double>>& d, int N, std::mt19937& rng) {
    std::uniform_int_distribution<int> uni(0, N - 1);
    int o = uni(rng);
    int a = farthest_from(o, d, N);
    int b = farthest_from(a, d, N);

    for (int iter = 0; iter < 2; ++iter) {
        int a2 = farthest_from(b, d, N);
        int b2 = farthest_from(a2, d, N);
        if (a2 == a && b2 == b) break; // heuristic has stabilized
        a = a2;
        b = b2;
    }

    if (a == b) {
        // Degenerate case (e.g. all remaining distances are zero): fall
        // back to any second distinct object so the projection is defined.
        b = (a + 1) % N;
    }
    return {a, b};
}

// Projects every object onto the line joining pivots (a, b) using the law
// of cosines, then deflates the distance matrix in place to remove the
// contribution already captured by this dimension.
static std::vector<double> project_and_deflate(std::vector<std::vector<double>>& d, int N, int a, int b) {
    std::vector<double> coord(N, 0.0);
    double dab = d[a][b];

    if (dab < 1e-9) {
        // Pivots (and hence the remaining spread) collapse to a point;
        // every object gets coordinate 0 on this axis and distances are
        // left untouched (nothing left to deflate).
        return coord;
    }

    for (int i = 0; i < N; ++i) {
        double dai = d[a][i];
        double dbi = d[b][i];
        coord[i] = (dai * dai + dab * dab - dbi * dbi) / (2.0 * dab);
    }
    coord[a] = 0.0;
    coord[b] = dab;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (i == j) continue;
            double diff = coord[i] - coord[j];
            double sq = d[i][j] * d[i][j] - diff * diff;
            d[i][j] = sq > 0.0 ? std::sqrt(sq) : 0.0;
        }
    }

    return coord;
}

FastMapResult run_fastmap(const std::vector<std::vector<double>>& dist, const FastMapParams& params) {
    FastMapResult result;
    int N = params.N;
    int K = params.K;

    result.coordinates.assign(N, std::vector<double>(K, 0.0));
    result.pivots.assign(K, {-1, -1});

    // Working copy gets deflated after each dimension.
    std::vector<std::vector<double>> working = dist;

    // Fixed seed: pivot selection is a randomized heuristic (Section 8.3
    // notes coordinates may differ slightly between correct
    // implementations), but a fixed seed keeps this program's own runs
    // reproducible.
    std::mt19937 rng(42);

    // Pivot selection, projection, and deflation for all K dimensions are
    // the timed algorithm section.
    TimePoint start = start_timer();

    for (int dim = 0; dim < K; ++dim) {
        std::pair<int, int> pivots = choose_pivots(working, N, rng);
        int a = pivots.first;
        int b = pivots.second;
        result.pivots[dim] = {a, b};

        std::vector<double> coord = project_and_deflate(working, N, a, b);
        for (int i = 0; i < N; ++i) {
            result.coordinates[i][dim] = coord[i];
        }
    }

    result.elapsed_time_ms = stop_timer(start);

    // Average pairwise distance error between original and embedded
    // distances (Section 8.3, optional reporting metric). Excluded from
    // the timed section.
    double total_error = 0.0;
    long long pair_count = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            double embedded_sq = 0.0;
            for (int k = 0; k < K; ++k) {
                double diff = result.coordinates[i][k] - result.coordinates[j][k];
                embedded_sq += diff * diff;
            }
            double embedded = std::sqrt(embedded_sq);
            total_error += std::fabs(embedded - dist[i][j]);
            pair_count++;
        }
    }
    result.avg_distance_error = pair_count > 0 ? total_error / pair_count : 0.0;

    return result;
}
