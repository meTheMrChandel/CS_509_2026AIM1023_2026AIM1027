#include "../include/kmeans.h"
#include "../include/timer.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>
#include <limits>
#include <algorithm>

static std::ifstream open_file(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::string alt = "tests/" + filename;
        fin.open(alt);
    }
    return fin;
}

bool read_kmeans_points(const std::string& filename, std::vector<std::vector<double>>& points, KMeansParams& params) {
    std::ifstream fin = open_file(filename);
    if (!fin.is_open()) {
        std::cerr << "Error: Unable to open file '" << filename << "'\n";
        return false;
    }

    if (!(fin >> params.N >> params.D >> params.K)) {
        std::cerr << "Error: Failed to read N, D, and K.\n";
        return false;
    }

    if (params.N <= 0 || params.D <= 0 || params.K <= 0) {
        std::cerr << "Error: N, D, and K must be positive.\n";
        return false;
    }

    if (params.K > params.N) {
        std::cerr << "Error: K (" << params.K << ") cannot be greater than N (" << params.N << ").\n";
        return false;
    }

    points.assign(params.N, std::vector<double>(params.D));
    for (int i = 0; i < params.N; ++i) {
        for (int j = 0; j < params.D; ++j) {
            if (!(fin >> points[i][j])) {
                std::cerr << "Error: Failed to read coordinate at point " << i << ", dimension " << j << ".\n";
                return false;
            }
        }
    }

    std::string key;
    bool has_max_iter = false;
    bool has_tolerance = false;

    while (fin >> key) {
        if (key == "MAX_ITERATIONS") {
            if (!(fin >> params.max_iterations)) {
                std::cerr << "Error: Failed to parse MAX_ITERATIONS.\n";
                return false;
            }
            has_max_iter = true;
        } else if (key == "TOLERANCE") {
            if (!(fin >> params.tolerance)) {
                std::cerr << "Error: Failed to parse TOLERANCE.\n";
                return false;
            }
            has_tolerance = true;
        }
    }

    if (has_max_iter) {
        if (params.max_iterations <= 0) {
            std::cerr << "Error: MAX_ITERATIONS must be positive.\n";
            return false;
        }
    } else {
        std::cerr << "Error: MAX_ITERATIONS parameter is missing.\n";
        return false;
    }

    if (has_tolerance) {
        if (params.tolerance <= 0.0) {
            std::cerr << "Error: TOLERANCE must be positive.\n";
            return false;
        }
    } else {
        std::cerr << "Error: TOLERANCE parameter is missing.\n";
        return false;
    }

    fin.close();
    return true;
}

KMeansResult run_kmeans(const std::vector<std::vector<double>>& points, const KMeansParams& params) {
    KMeansResult result;
    int N = params.N;
    int D = params.D;
    int K = params.K;
    int max_iter = params.max_iterations;
    double tol = params.tolerance;

    result.assignments.assign(N, -1);
    result.centroids.assign(K, std::vector<double>(D));
    for (int k = 0; k < K; ++k) {
        result.centroids[k] = points[k];
    }

    // Start timing only the algorithm execution time
    TimePoint start = start_timer();

    int iter = 0;
    bool converged = false;

    while (iter < max_iter) {
        int changes = 0;

        // 1. Assignment step
        for (int i = 0; i < N; ++i) {
            double min_dist_sq = std::numeric_limits<double>::max();
            int best_centroid = -1;

            for (int k = 0; k < K; ++k) {
                double dist_sq = 0.0;
                for (int j = 0; j < D; ++j) {
                    double diff = points[i][j] - result.centroids[k][j];
                    dist_sq += diff * diff;
                }

                if (dist_sq < min_dist_sq) {
                    min_dist_sq = dist_sq;
                    best_centroid = k;
                }
            }

            if (result.assignments[i] != best_centroid) {
                result.assignments[i] = best_centroid;
                changes++;
            }
        }

        // 2. Update step
        std::vector<std::vector<double>> new_centroids(K, std::vector<double>(D, 0.0));
        std::vector<int> cluster_sizes(K, 0);

        for (int i = 0; i < N; ++i) {
            int c = result.assignments[i];
            cluster_sizes[c]++;
            for (int j = 0; j < D; ++j) {
                new_centroids[c][j] += points[i][j];
            }
        }

        double max_shift = 0.0;
        for (int k = 0; k < K; ++k) {
            if (cluster_sizes[k] == 0) {
                new_centroids[k] = result.centroids[k];
            } else {
                for (int j = 0; j < D; ++j) {
                    new_centroids[k][j] /= cluster_sizes[k];
                }
            }

            // Shift distance check
            double shift_sq = 0.0;
            for (int j = 0; j < D; ++j) {
                double diff = new_centroids[k][j] - result.centroids[k][j];
                shift_sq += diff * diff;
            }
            double shift = std::sqrt(shift_sq);
            if (shift > max_shift) {
                max_shift = shift;
            }
        }

        result.centroids = new_centroids;
        iter++;

        if (changes == 0 || max_shift <= tol) {
            converged = true;
            break;
        }
    }

    result.elapsed_time_ms = stop_timer(start);
    result.iterations = iter;
    result.converged = converged;

    // Calculate WCSS
    double wcss = 0.0;
    for (int i = 0; i < N; ++i) {
        int c = result.assignments[i];
        double dist_sq = 0.0;
        for (int j = 0; j < D; ++j) {
            double diff = points[i][j] - result.centroids[c][j];
            dist_sq += diff * diff;
        }
        wcss += dist_sq;
    }
    result.wcss = wcss;

    return result;
}
