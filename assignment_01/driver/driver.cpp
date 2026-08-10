#include "../include/bfs.h"
#include "../include/csr.h"
#include "../include/dfs.h"
#include "../include/graph.h"
#include "../include/sssp.h"
#include "../include/timer.h"

#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

// Displays command help menu in terminal.
static void print_usage()
{
    std::cout << "Usage:\n";
    std::cout << "  graph_simulator <bfs|dfs|sssp> <test_file>\n";
    std::cout << "  graph_simulator --all-tests <bfs|dfs|sssp>\n";
    std::cout << "  graph_simulator --all\n";
    std::cout << "  graph_simulator (interactive mode)\n";
}

// Outputs results from a BFS execution run.
static void print_bfs_output(int i_source, const BFSResult& struct_result, double d_elapsed_ms)
{
    std::cout << "Algorithm: BFS\n";
    std::cout << "Source: " << i_source << "\n";
    std::cout << "Traversal:";

    for (int i_vertex : struct_result.v_traversal)
    {
        std::cout << " " << i_vertex;
    }

    std::cout << "\nDistances:\n";

    for (size_t i_idx = 0; i_idx < struct_result.v_distances.size(); i_idx++)
    {
        std::cout << i_idx << " ";

        if (struct_result.v_distances[i_idx] == -1)
        {
            std::cout << "INF";
        }
        else
        {
            std::cout << struct_result.v_distances[i_idx];
        }

        std::cout << "\n";
    }

    std::cout << "Execution time: " << d_elapsed_ms << " ms\n";
}

// Outputs results from a DFS execution run.
static void print_dfs_output(int i_source, const DFSResult& struct_result, double d_elapsed_ms)
{
    std::cout << "Algorithm: DFS\n";
    std::cout << "Source: " << i_source << "\n";
    std::cout << "Traversal:";

    for (int i_vertex : struct_result.v_traversal)
    {
        std::cout << " " << i_vertex;
    }

    std::cout << "\nExecution time: " << d_elapsed_ms << " ms\n";
}

// Outputs results from an SSSP execution run.
static void print_sssp_output(int i_source, const SSSPResult& struct_result, double d_elapsed_ms)
{
    std::cout << "Algorithm: SSSP\n";
    std::cout << "Source: " << i_source << "\n";
    std::cout << "Vertex Distance\n";

    for (size_t i_idx = 0; i_idx < struct_result.v_distances.size(); i_idx++)
    {
        std::cout << i_idx << "\t";

        if (struct_result.v_distances[i_idx] == std::numeric_limits<int>::max())
        {
            std::cout << "INF";
        }
        else
        {
            std::cout << struct_result.v_distances[i_idx];
        }

        std::cout << "\n";
    }

    std::cout << "Execution time: " << d_elapsed_ms << " ms\n";
}

// Runs specified algorithm on input test case file.
static int run_algorithm_on_file(const std::string& str_algorithm, const std::string& str_test_file)
{
    GraphInput struct_input;
    bool b_status = false;

    // Delegate reading to weighted/unweighted loaders based on algorithm requirements.
    if (str_algorithm == "bfs" || str_algorithm == "dfs")
    {
        b_status = read_unweighted_graph(str_test_file, struct_input);
    }
    else if (str_algorithm == "sssp")
    {
        b_status = read_weighted_graph(str_test_file, struct_input);
    }
    else
    {
        std::cerr << "Error: Unknown algorithm '" << str_algorithm << "'.\n";
        return 1;
    }

    if (!b_status)
    {
        return 1;
    }

    // Convert loaded adjacency representations to flat Compressed Sparse Row format.
    // Excluded from measured algorithm runtime.
    CSRGraph struct_graph = convert_to_csr(struct_input.v_adj_list);

    std::cout << "\nTest file: " << str_test_file << "\n";
    std::cout << "Vertices: " << struct_input.i_vertex_count << ", Edges: " << struct_input.i_edge_count << "\n\n";

    TimePoint t_start = start_timer();

    if (str_algorithm == "bfs")
    {
        BFSResult struct_result = bfs(struct_graph, struct_input.i_source);
        double d_elapsed_ms = stop_timer(t_start);
        print_bfs_output(struct_input.i_source, struct_result, d_elapsed_ms);
    }
    else if (str_algorithm == "dfs")
    {
        DFSResult struct_result = dfs(struct_graph, struct_input.i_source);
        double d_elapsed_ms = stop_timer(t_start);
        print_dfs_output(struct_input.i_source, struct_result, d_elapsed_ms);
    }
    else
    {
        SSSPResult struct_result = dijkstra(struct_graph, struct_input.i_source);
        double d_elapsed_ms = stop_timer(t_start);
        print_sssp_output(struct_input.i_source, struct_result, d_elapsed_ms);
    }

    std::cout << std::endl;
    return 0;
}

// Verification helper for file existence.
static bool test_file_exists(const std::string& str_path)
{
    std::ifstream stream_fin(str_path);
    return stream_fin.good();
}

// Scans for files matching prefix at all required vertex count levels.
static std::vector<std::string> collect_test_files(const std::string& str_prefix)
{
    std::vector<std::string> v_files;
    const int i_scales[] = {10, 100, 10000, 50000, 100000};

    for (int i_scale : i_scales)
    {
        std::string str_path = "tests/" + str_prefix + std::to_string(i_scale) + ".txt";
        if (test_file_exists(str_path))
        {
            v_files.push_back(str_path);
        }
    }

    return v_files;
}

// Runs tests across all scales for a selected algorithm.
static int run_all_tests_for_algorithm(const std::string& str_algorithm)
{
    std::vector<std::string> v_test_files = collect_test_files(str_algorithm + "_");

    if (v_test_files.empty())
    {
        std::cerr << "Error: No test files found for algorithm " << str_algorithm << ".\n";
        return 1;
    }

    for (const std::string& str_test_file : v_test_files)
    {
        if (run_algorithm_on_file(str_algorithm, str_test_file) != 0)
        {
            return 1;
        }
    }

    return 0;
}

// Evaluates all algorithms sequentially across all test files.
static int run_all_algorithms_on_all_tests()
{
    const std::vector<std::string> v_algorithms = {"bfs", "dfs", "sssp"};

    for (const std::string& str_algorithm : v_algorithms)
    {
        if (run_all_tests_for_algorithm(str_algorithm) != 0)
        {
            return 1;
        }
    }

    return 0;
}

// CLI menu for interactive terminal sessions.
static int run_interactive()
{
    std::string str_algorithm;
    std::string str_test_file;

    std::cout << "Select algorithm (bfs/dfs/sssp): ";
    std::cin >> str_algorithm;

    std::cout << "Enter test file path (e.g. tests/bfs_10.txt): ";
    std::cin >> str_test_file;

    return run_algorithm_on_file(str_algorithm, str_test_file);
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        return run_interactive();
    }

    if (argc >= 2 && std::string(argv[1]) == "--all")
    {
        return run_all_algorithms_on_all_tests();
    }

    if (argc >= 2 && std::string(argv[1]) == "--all-tests")
    {
        if (argc < 3)
        {
            print_usage();
            return 1;
        }

        return run_all_tests_for_algorithm(argv[2]);
    }

    if (argc < 3)
    {
        print_usage();
        return 1;
    }

    return run_algorithm_on_file(argv[1], argv[2]);
}
