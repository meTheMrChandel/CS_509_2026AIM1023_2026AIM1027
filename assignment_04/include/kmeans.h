#ifndef KMEANS_H
#define KMEANS_H

#include <vector>
#include <string>

struct KMeansParams {
    int N = 0;
    int D = 0;
    int K = 0;
    int max_iterations = 300;
    double tolerance = 0.0001;
};

struct KMeansResult {
    std::vector<int> assignments;               // Cluster assignment for each point. Size: N.
    std::vector<std::vector<double>> centroids; // Final coordinates of the K centroids.
    double wcss = 0.0;
    int iterations = 0;
    bool converged = false;
    double elapsed_time_ms = 0.0;
};

// Reads K-Means points and parameters from a file
bool read_kmeans_points(const std::string& filename, std::vector<std::vector<double>>& points, KMeansParams& params);

// Runs K-Means Clustering on the provided points
KMeansResult run_kmeans(const std::vector<std::vector<double>>& points, const KMeansParams& params);

#endif // KMEANS_H
