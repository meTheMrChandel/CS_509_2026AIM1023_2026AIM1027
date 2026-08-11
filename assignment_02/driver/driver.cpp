#include "../include/graph.h"
#include "../include/connected_components.h"
#include "../include/triangle_counting.h"
#include "../include/betweenness_centrality.h"
#include "../include/timer.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

static void print_usage() {
    std::cout << "Usage:\n";
    std::cout << "  graph_simulator <tc|cc|bc> <test_file>\n";
}

int main(int argc, char* argv[]) {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::string algorithm;
    std::string test_file;

    if (argc < 3) {
        int algo_choice = -1;
        std::cout << "\nSelect Algorithm:\n";
        std::cout << "1. Triangle Counting (tc)\n";
        std::cout << "2. Connected Components (cc)\n";
        std::cout << "3. Betweenness Centrality (bc)\n";
        std::cout << "Enter choice (1-3): ";
        if (!(std::cin >> algo_choice)) {
            return 1;
        }

        if (algo_choice == 1) {
            algorithm = "tc";
        } else if (algo_choice == 2) {
            algorithm = "cc";
        } else if (algo_choice == 3) {
            algorithm = "bc";
        } else {
            std::cerr << "Error: Invalid choice.\n";
            return 1;
        }

        std::cout << "Enter test file path: ";
        if (!(std::cin >> test_file)) {
            return 1;
        }
    } else {
        algorithm = argv[1];
        test_file = argv[2];
    }

    if (algorithm != "tc" && algorithm != "cc" && algorithm != "bc") {
        std::cerr << "Error: Unknown algorithm '" << algorithm << "'. Use 'tc', 'cc', or 'bc'.\n";
        print_usage();
        return 1;
    }

    GraphInput input;
    if (!read_undirected_graph(test_file, input)) {
        std::cerr << "Error: Failed to read graph from file '" << test_file << "'.\n";
        return 1;
    }

    if (algorithm == "tc") {
        TimePoint start = start_timer();
        TriangleResult result = count_triangles(input.v_adj_list);
        double elapsed_us = stop_timer_us(start);

        std::cout << "Algorithm: Triangle Counting\n";
        std::cout << "Total triangles: " << result.total_triangles << "\n";
        if (input.i_vertex_count <= 100) {
            std::cout << "Triangles found:\n";
            for (const auto& t : result.triangles_found) {
                std::cout << "(" << t.u << ", " << t.v << ", " << t.w << ")\n";
            }
        }
        std::cout << "Execution time: " << elapsed_us << " us\n";

    } else if (algorithm == "cc") {
        TimePoint start = start_timer();
        CCResult result = find_connected_components(input.v_adj_list);
        double elapsed_us = stop_timer_us(start);

        std::cout << "Algorithm: Connected Components\n";
        std::cout << "Number of components: " << result.total_components << "\n";
        std::cout << "Vertex Component\n";
        for (int i = 0; i < input.i_vertex_count; ++i) {
            std::cout << i << " " << result.v_components[i] << "\n";
        }
        std::cout << "Execution time: " << elapsed_us << " us\n";

    } else if (algorithm == "bc") {
        TimePoint start = start_timer();
        BCResult result = compute_betweenness_centrality(input.v_adj_list);
        double elapsed_us = stop_timer_us(start);

        std::cout << "Algorithm: Betweenness Centrality\n";
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "Vertex Centrality\n";
        for (int i = 0; i < input.i_vertex_count; ++i) {
            std::cout << i << " " << result.v_centrality[i] << "\n";
        }
        std::cout << "Execution time: " << elapsed_us << " us\n";
    }

    return 0;
}
