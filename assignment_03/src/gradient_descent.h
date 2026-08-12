#ifndef GRADIENT_DESCENT_H
#define GRADIENT_DESCENT_H

#include <vector>
#include <string>

struct OptimizationConfig {
    int polynomial_degree = 0;
    std::vector<double> coefficients;
    double initial_x = 0.0;
    double learning_rate = 0.0;
    double convergence_tolerance = 0.0;
    int max_iterations = 0;
};

struct OptimizationResult {
    double final_x = 0.0;
    double final_fx = 0.0;
    int iterations_completed = 0;
    bool is_converged = false;
    long long elapsed_time_microseconds = 0;
};

class GradientDescentSolver {
public:
    static OptimizationConfig parse_configuration(const std::string& filepath);
    static double evaluate_polynomial(const std::vector<double>& coefficients, double x);
    static double evaluate_derivative(const std::vector<double>& coefficients, double x);
    static OptimizationResult optimize(const OptimizationConfig& config);
};

#endif
