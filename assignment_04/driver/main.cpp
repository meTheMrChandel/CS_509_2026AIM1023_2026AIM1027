#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "../include/graph.h"
#include "../include/csr.h"
#include "../include/graph_coloring.h"
#include "../include/pagerank.h"
#include "../include/kmeans.h"
#include "../include/fastmap.h"

int main()
{
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cout << "=========================================\n";
    std::cout << "Select the Algorithm:\n";
    std::cout << "1. Vertex Coloring (Graph Coloring)\n";
    std::cout << "2. PageRank\n";
    std::cout << "3. K-Means Clustering\n";
    std::cout << "4. FastMap\n";
    std::cout << "=========================================\n";
    std::cout << "Enter choice (1-4): ";

    int choice;
    if (!(std::cin >> choice))
    {
        std::cerr << "Error: Invalid selection.\n";
        return 1;
    }

    if (choice < 1 || choice > 4)
    {
        std::cerr << "Error: Selection must be between 1 and 4.\n";
        return 1;
    }

    std::string filepath;
    std::cout << "Enter the path to the input file: ";
    if (!(std::cin >> filepath))
    {
        std::cerr << "Error: Invalid input path.\n";
        return 1;
    }

    if (choice == 1)
    {
        int V = 0, E = 0;
        AdjList adj;
        if (!read_vertex_coloring_graph(filepath, V, E, adj))
        {
            return 1;
        }

        // Convert Adjacency List to CSR
        std::vector<int> row_ptr;
        std::vector<int> col_idx;
        convert_to_csr(adj, row_ptr, col_idx);

        // Run coloring
        ColoringResult res = run_greedy_vertex_coloring(V, row_ptr, col_idx);

        if (!res.is_valid)
        {
            std::cerr << "Error: Computed coloring is invalid!\n";
            return 1;
        }

        std::cout << "\nAlgorithm: Greedy Vertex Coloring\n";
        std::cout << "Vertex colors:\n";
        for (int i = 0; i < V; ++i)
        {
            std::cout << i << " " << res.colors[i] << "\n";
        }
        std::cout << "Colors used: " << res.colors_used << "\n";
        std::cout << "Execution time: " << res.elapsed_time_ms << " ms\n";
    }
    else if (choice == 2)
    {
        int V = 0, E = 0;
        AdjList adj;
        PageRankParams params;
        if (!read_pagerank_graph(filepath, V, E, adj, params))
        {
            return 1;
        }

        // Convert Adjacency List to CSR
        std::vector<int> row_ptr;
        std::vector<int> col_idx;
        convert_to_csr(adj, row_ptr, col_idx);

        // Run PageRank
        PageRankResult res = run_pagerank(V, row_ptr, col_idx, params);

        double sum_ranks = 0.0;
        for (double r : res.ranks)
        {
            sum_ranks += r;
        }

        std::cout << "\nAlgorithm: PageRank\n";
        std::cout << "Damping: " << params.damping << "\n";
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "Vertex ranks:\n";
        for (int i = 0; i < V; ++i)
        {
            std::cout << i << " " << res.ranks[i] << "\n";
        }
        std::cout << "Sum of ranks: " << sum_ranks << "\n";
        std::cout << "Iterations: " << res.iterations << "\n";
        std::cout << "Converged: " << (res.converged ? "true" : "false") << "\n";
        std::cout << std::defaultfloat << std::setprecision(6); // reset formatting
        std::cout << "Execution time: " << res.elapsed_time_ms << " ms\n";
    }
    else if (choice == 3)
    {
        std::vector<std::vector<double>> points;
        KMeansParams params;
        if (!read_kmeans_points(filepath, points, params))
        {
            return 1;
        }

        KMeansResult res = run_kmeans(points, params);

        std::cout << "\nAlgorithm: K-Means Clustering\n";
        std::cout << "K: " << params.K << "\n";
        std::cout << "Point assignments:\n";
        for (int i = 0; i < params.N; ++i)
        {
            std::cout << i << " " << res.assignments[i] << "\n";
        }
        std::cout << "Final centroids:\n";
        std::cout << std::fixed << std::setprecision(6);
        for (int k = 0; k < params.K; ++k)
        {
            std::cout << k << ":";
            for (int j = 0; j < params.D; ++j)
            {
                std::cout << " " << res.centroids[k][j];
            }
            std::cout << "\n";
        }
        std::cout << "WCSS: " << res.wcss << "\n";
        std::cout << "Iterations: " << res.iterations << "\n";
        std::cout << "Converged: " << (res.converged ? "true" : "false") << "\n";
        std::cout << std::defaultfloat << std::setprecision(6); // reset formatting
        std::cout << "Execution time: " << res.elapsed_time_ms << " ms\n";
    }
    else if (choice == 4)
    {
        std::vector<std::vector<double>> dist;
        FastMapParams params;
        if (!read_fastmap_input(filepath, dist, params))
        {
            return 1;
        }

        FastMapResult res = run_fastmap(dist, params);

        std::cout << "\nAlgorithm: FastMap\n";
        std::cout << "Target dimensions: " << params.K << "\n";
        std::cout << "Pivots per dimension:\n";
        for (int k = 0; k < params.K; ++k)
        {
            std::cout << "Dim " << (k + 1) << ": " << res.pivots[k].first
                      << " " << res.pivots[k].second << "\n";
        }
        std::cout << "Object coordinates:\n";
        std::cout << std::fixed << std::setprecision(6);
        for (int i = 0; i < params.N; ++i)
        {
            std::cout << i << ":";
            for (int k = 0; k < params.K; ++k)
            {
                std::cout << " " << res.coordinates[i][k];
            }
            std::cout << "\n";
        }
        std::cout << "Average distance error: " << res.avg_distance_error << "\n";
        std::cout << std::defaultfloat << std::setprecision(6); // reset formatting
        std::cout << "Execution time: " << res.elapsed_time_ms << " ms\n";
    }

    return 0;
}
