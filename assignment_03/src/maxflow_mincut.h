#ifndef MAXFLOW_MINCUT_H
#define MAXFLOW_MINCUT_H

#include <string>
#include <vector>

struct MaxflowEdge {
    int source;
    int destination;
    long long capacity;
};

struct MaxflowAdjacencyInput {
    int vertex_count;
    int source;
    int sink;
    std::vector<MaxflowEdge> edges;
};

struct MaxflowCSRGraph {
    int vertex_count;
    int source;
    int sink;
    std::vector<int> row_ptr;
    std::vector<int> col_index;
    std::vector<long long> capacities;
};

class MaxflowMincutSolver {
public:
    static MaxflowAdjacencyInput parse_input(const std::string& filepath);
    static MaxflowCSRGraph convert_to_csr(const MaxflowAdjacencyInput& input);
    static void solve(const MaxflowCSRGraph& csr_graph);
};

#endif
