#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Helper to run shell command and output command trace.
static int run_command(const std::string& str_command)
{
    std::cout << "\n> " << str_command << "\n";
    return std::system(str_command.c_str());
}

// Verifies file existence on local storage.
static bool file_exists(const std::string& str_path)
{
    std::ifstream stream_fin(str_path);
    return stream_fin.good();
}

// Generates a list of all graph test files found in tests folder matching a prefix.
static std::vector<std::string> list_test_files(const std::string& str_prefix)
{
    std::vector<std::string> v_files;
    const int i_scales[] = {10, 100, 10000, 50000, 100000};

    for (int i_scale : i_scales)
    {
        std::string str_filename = str_prefix + std::to_string(i_scale) + ".txt";
        if (file_exists("../assignment_01/tests/" + str_filename))
        {
            v_files.push_back(str_filename);
        }
    }

    return v_files;
}

// Invokes Makefile compilation for Assignment 01 in buddy repo.
static void compile_assignment_01()
{
    if (run_command("mingw32-make -C ..\\assignment_01") != 0)
    {
        std::cout << "Error: Compilation failed for Assignment 01.\n";
    }
    else
    {
        std::cout << "Assignment 01 compiled successfully.\n";
    }
}

// Interactively prompts parameters to execute a single test case.
static void run_single_test()
{
    const std::string str_executable = "..\\assignment_01\\graph_simulator.exe";

    if (!file_exists(str_executable))
    {
        std::cout << "Error: Executable not found. Compile Assignment 01 first.\n";
        return;
    }

    std::string str_algorithm;
    std::string str_test_file;

    std::cout << "Select algorithm (bfs/dfs/sssp): ";
    std::cin >> str_algorithm;

    std::cout << "Enter test file name (e.g. bfs_10.txt): ";
    std::cin >> str_test_file;

    run_command(str_executable + " " + str_algorithm + " tests\\" + str_test_file);
}

// Configures and runs all test cases for a single algorithm.
static void run_all_tests_for_algorithm()
{
    const std::string str_executable = "..\\assignment_01\\graph_simulator.exe";

    if (!file_exists(str_executable))
    {
        std::cout << "Error: Executable not found. Compile Assignment 01 first.\n";
        return;
    }

    std::string str_algorithm;
    std::cout << "Select algorithm (bfs/dfs/sssp): ";
    std::cin >> str_algorithm;

    run_command(str_executable + " --all-tests " + str_algorithm);
}

// Runs all algorithms on all test files.
static void run_all_algorithms()
{
    const std::string str_executable = "..\\assignment_01\\graph_simulator.exe";

    if (!file_exists(str_executable))
    {
        std::cout << "Error: Executable not found. Compile Assignment 01 first.\n";
        return;
    }

    run_command(str_executable + " --all");
}

// Submenu interface for Assignment 01.
static void show_assignment_01_menu()
{
    int i_choice = -1;

    while (i_choice != 0)
    {
        std::cout << "\n===== Assignment 01: Graph Algorithms =====\n";
        std::cout << "1. Compile Assignment 01\n";
        std::cout << "2. Run one test file\n";
        std::cout << "3. Run all test files for one algorithm\n";
        std::cout << "4. Run all algorithms on all test files\n";
        std::cout << "5. List available test files\n";
        std::cout << "0. Back\n";
        std::cout << "Enter choice: ";
        std::cin >> i_choice;

        switch (i_choice)
        {
            case 1:
                compile_assignment_01();
                break;
            case 2:
                run_single_test();
                break;
            case 3:
                run_all_tests_for_algorithm();
                break;
            case 4:
                run_all_algorithms();
                break;
            case 5:
            {
                const std::vector<std::string> v_prefixes = {"bfs_", "dfs_", "sssp_"};
                for (const std::string& str_prefix : v_prefixes)
                {
                    std::vector<std::string> v_files = list_test_files(str_prefix);
                    std::cout << str_prefix << " tests:\n";
                    if (v_files.empty())
                    {
                        std::cout << "  (none)\n";
                    }
                    else
                    {
                        for (const std::string& str_file : v_files)
                        {
                            std::cout << "  - " << str_file << "\n";
                        }
                    }
                }
                break;
            }
            default:
                if (i_choice != 0)
                {
                    std::cout << "Invalid choice.\n";
                }
                break;
        }
    }
}

int main()
{
    int i_choice = -1;

    while (i_choice != 0)
    {
        std::cout << "\n=====================================\n";
        std::cout << "     CS509 Buddy Repository Menu\n";
        std::cout << "=====================================\n";
        std::cout << "1. Assignment 01 - BFS, DFS, SSSP\n";
        std::cout << "2. Assignment 02 (not available)\n";
        std::cout << "3. Assignment 03 (not available)\n";
        std::cout << "4. Assignment 04 (not available)\n";
        std::cout << "5. Assignment 05 (not available)\n";
        std::cout << "6. Assignment 06 (not available)\n";
        std::cout << "7. Assignment 07 (not available)\n";
        std::cout << "8. Assignment 08 (not available)\n";
        std::cout << "9. Assignment 09 (not available)\n";
        std::cout << "10. Assignment 10 (not available)\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> i_choice;

        switch (i_choice)
        {
            case 1:
                show_assignment_01_menu();
                break;
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
                std::cout << "Assignment " << i_choice << " is not available yet.\n";
                break;
            case 0:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice.\n";
                break;
        }
    }

    return 0;
}
