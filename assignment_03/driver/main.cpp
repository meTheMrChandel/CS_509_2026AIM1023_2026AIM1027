#include "../src/gradient_descent.h"
#include "../src/maxflow_mincut.h"
#include <iostream>
#include <string>
#include <iomanip>

int main() {
    std::cout << "=========================================\n";
    std::cout << "Select the Algorithm:\n";
    std::cout << "1. Gradient Descent\n";
    std::cout << "2. Maxflow-Mincut\n";
    std::cout << "=========================================\n";
    std::cout << "Enter choice (1-2): ";

    int choice;
    if (!(std::cin >> choice)) {
        std::cerr << "Error: Invalid selection.\n";
        return 1;
    }

    std::string filepath;
    std::cout << "Enter the path to the input file (e.g., tests/gd_01.txt, tests/gd_02.txt, tests/gd_03.txt, tests/gd_04.txt, tests/gd_05.txt): ";
    if (!(std::cin >> filepath)) {
        std::cerr << "Error: Invalid input path.\n";
        return 1;
    }

    if (choice == 1) {
        try {
            OptimizationConfig config = GradientDescentSolver::parse_configuration(filepath);
            OptimizationResult result = GradientDescentSolver::optimize(config);

            std::cout << "\nAlgorithm: Gradient Descent\n";
            std::cout << "Degree: " << config.polynomial_degree << "\n";
            std::cout << std::fixed << std::setprecision(6);
            std::cout << "Final x: approximately " << result.final_x << "\n";
            std::cout << "Final f(x): approximately " << result.final_fx << "\n";
            std::cout << "Iterations: " << result.iterations_completed << "\n";
            std::cout << "Converged: " << (result.is_converged ? "true" : "false") << "\n";
            std::cout << "Execution time: " << result.elapsed_time_microseconds << " microseconds\n";
        } catch (const std::exception& e) {
            std::cerr << "\nError: " << e.what() << "\n";
            return 1;
        }
    } else if (choice == 2) {
        MaxflowMincutSolver::solve(filepath);
    } else {
        std::cerr << "Error: Selection must be 1 or 2.\n";
        return 1;
    }

    return 0;
}
