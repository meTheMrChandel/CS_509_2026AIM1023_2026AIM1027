#include "gradient_descent.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <chrono>

OptimizationConfig GradientDescentSolver::parse_configuration(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }

    OptimizationConfig config;
    std::string line;
    bool has_degree = false, has_coeffs = false, has_initial_x = false;
    bool has_learning_rate = false, has_tolerance = false, has_max_iter = false;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string key;
        if (!(ss >> key)) continue;

        if (key == "DEGREE") {
            if (!(ss >> config.polynomial_degree) || config.polynomial_degree < 0) {
                throw std::invalid_argument("Invalid degree value");
            }
            has_degree = true;
        } else if (key == "COEFFICIENTS") {
            double coeff;
            while (ss >> coeff) {
                config.coefficients.push_back(coeff);
            }
            has_coeffs = true;
        } else if (key == "INITIAL_X") {
            if (!(ss >> config.initial_x)) {
                throw std::invalid_argument("Invalid initial_x value");
            }
            has_initial_x = true;
        } else if (key == "LEARNING_RATE") {
            if (!(ss >> config.learning_rate) || config.learning_rate <= 0) {
                throw std::invalid_argument("Invalid learning_rate value");
            }
            has_learning_rate = true;
        } else if (key == "TOLERANCE") {
            if (!(ss >> config.convergence_tolerance) || config.convergence_tolerance <= 0) {
                throw std::invalid_argument("Invalid tolerance value");
            }
            has_tolerance = true;
        } else if (key == "MAX_ITERATIONS") {
            if (!(ss >> config.max_iterations) || config.max_iterations <= 0) {
                throw std::invalid_argument("Invalid max_iterations value");
            }
            has_max_iter = true;
        }
    }

    if (!has_degree || !has_coeffs || !has_initial_x || !has_learning_rate || !has_tolerance || !has_max_iter) {
        throw std::invalid_argument("Input file is missing required parameters");
    }

    if (config.coefficients.size() != static_cast<size_t>(config.polynomial_degree + 1)) {
        throw std::invalid_argument("Coefficient count must equal degree + 1");
    }

    return config;
}

double GradientDescentSolver::evaluate_polynomial(const std::vector<double>& coefficients, double x) {
    double value = 0.0;
    for (auto it = coefficients.rbegin(); it != coefficients.rend(); ++it) {
        value = value * x + *it;
    }
    return value;
}

double GradientDescentSolver::evaluate_derivative(const std::vector<double>& coefficients, double x) {
    double value = 0.0;
    for (int i = static_cast<int>(coefficients.size()) - 1; i >= 1; --i) {
        value = value * x + i * coefficients[i];
    }
    return value;
}

OptimizationResult GradientDescentSolver::optimize(const OptimizationConfig& config) {
    double x = config.initial_x;
    int iterations = 0;
    bool converged = false;

    auto start_time = std::chrono::high_resolution_clock::now();

    while (iterations < config.max_iterations) {
        double derivative = evaluate_derivative(config.coefficients, x);
        if (std::abs(derivative) <= config.convergence_tolerance) {
            converged = true;
            break;
        }
        x = x - config.learning_rate * derivative;
        iterations++;
    }

    // Final convergence check at the final point if we hit max iterations
    if (!converged) {
        double derivative = evaluate_derivative(config.coefficients, x);
        if (std::abs(derivative) <= config.convergence_tolerance) {
            converged = true;
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();

    OptimizationResult result;
    result.final_x = x;
    result.final_fx = evaluate_polynomial(config.coefficients, x);
    result.iterations_completed = iterations;
    result.is_converged = converged;
    result.elapsed_time_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    return result;
}
