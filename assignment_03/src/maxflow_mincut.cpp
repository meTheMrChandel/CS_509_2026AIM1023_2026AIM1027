#include "maxflow_mincut.h"

#include <cctype>
#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

struct Edge {
    int to, rev;
    long long cap, orig;
};

struct CutEdge {
    int from;
    int to;
    long long capacity;
};

class Dinic {
public:
    Dinic(int n, int s, int t) : n_(n), s_(s), t_(t), g_(n) {}

    void addEdge(int u, int v, long long c) {
        g_[u].push_back({v, static_cast<int>(g_[v].size()), c, c});
        g_[v].push_back({u, static_cast<int>(g_[u].size()) - 1, 0, 0});
    }

    long long maxFlow() {
        long long flow = 0;
        while (bfs()) {
            it_.assign(n_, 0);
            while (long long pushed = dfs(s_, std::numeric_limits<long long>::max())) {
                flow += pushed;
            }
        }
        return flow;
    }

    long long minCut() const {
        std::vector<int> vis;
        long long cut = 0;
        std::vector<CutEdge> edges;
        computeCutData(vis, cut, edges);
        return cut;
    }

    std::vector<int> sourceReachable() const {
        std::vector<int> vis(n_, 0);
        std::queue<int> q;
        q.push(s_);
        vis[s_] = 1;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (const auto& e : g_[u]) {
                if (e.cap > 0 && !vis[e.to]) {
                    vis[e.to] = 1;
                    q.push(e.to);
                }
            }
        }
        return vis;
    }

    std::vector<CutEdge> cutEdges() const {
        std::vector<int> vis;
        long long cut = 0;
        std::vector<CutEdge> edges;
        computeCutData(vis, cut, edges);
        return edges;
    }

    void computeCutData(std::vector<int>& vis, long long& cut, std::vector<CutEdge>& edges) const {
        vis = sourceReachable();
        cut = 0;
        edges.clear();
        for (int u = 0; u < n_; ++u) {
            if (!vis[u]) continue;
            for (const auto& e : g_[u]) {
                if (e.orig > 0 && !vis[e.to]) {
                    cut += e.orig;
                    edges.push_back({u, e.to, e.orig});
                }
            }
        }
    }

private:
    bool bfs() {
        level_.assign(n_, -1);
        std::queue<int> q;
        q.push(s_);
        level_[s_] = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (const auto& e : g_[u]) {
                if (e.cap > 0 && level_[e.to] < 0) {
                    level_[e.to] = level_[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level_[t_] >= 0;
    }

    long long dfs(int u, long long f) {
        if (u == t_) return f;
        for (int& i = it_[u]; i < static_cast<int>(g_[u].size()); ++i) {
            Edge& e = g_[u][i];
            if (e.cap <= 0 || level_[e.to] != level_[u] + 1) continue;
            long long pushed = dfs(e.to, std::min(f, e.cap));
            if (pushed > 0) {
                e.cap -= pushed;
                g_[e.to][e.rev].cap += pushed;
                return pushed;
            }
        }
        return 0;
    }

    int n_, s_, t_;
    std::vector<std::vector<Edge>> g_;
    std::vector<int> level_, it_;
};

MaxflowAdjacencyInput parseAdjacencyInput(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) throw std::runtime_error("Failed to open file: " + path);

    int n = 0, expected_edges = 0;
    if (!(in >> n >> expected_edges)) {
        throw std::runtime_error("Invalid header. Expected: n m");
    }
    if (n <= 0 || expected_edges < 0) {
        throw std::runtime_error("Invalid graph size in header.");
    }

    MaxflowAdjacencyInput input;
    input.vertex_count = n;
    input.source = 0;
    input.sink = n - 1;

    for (int row = 0; row < n; ++row) {
        int u = -1, out_degree = -1;
        if (!(in >> u >> out_degree)) {
            throw std::runtime_error("Invalid adjacency row.");
        }
        if (u < 0 || u >= n || out_degree < 0) {
            throw std::runtime_error("Invalid node id or out degree in adjacency row.");
        }

        for (int i = 0; i < out_degree; ++i) {
            int v = -1;
            long long c = 0;
            if (!(in >> v >> c)) {
                throw std::runtime_error("Invalid edge pair in adjacency row.");
            }
            if (v < 0 || v >= n || c <= 0) {
                throw std::runtime_error("Invalid edge endpoint or capacity.");
            }
            input.edges.push_back({u, v, c});
        }
    }

    std::string source_label;
    if (!(in >> source_label >> input.source)) {
        throw std::runtime_error("Missing SOURCE line.");
    }
    for (char& ch : source_label) ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    if (source_label != "SOURCE" && source_label != "SRC" && source_label != "S") {
        throw std::runtime_error("Invalid SOURCE label.");
    }

    std::string sink_label;
    if (!(in >> sink_label >> input.sink)) {
        throw std::runtime_error("Missing SINK line.");
    }
    for (char& ch : sink_label) ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    if (sink_label != "SINK" && sink_label != "SNK" && sink_label != "T") {
        throw std::runtime_error("Invalid SINK label.");
    }

    if (input.source < 0 || input.source >= n || input.sink < 0 || input.sink >= n || input.source == input.sink) {
        throw std::runtime_error("Invalid source/sink in input file.");
    }
    if (static_cast<int>(input.edges.size()) != expected_edges) {
        throw std::runtime_error("Edge count does not match header declaration.");
    }

    return input;
}

MaxflowCSRGraph convertToCSR(const MaxflowAdjacencyInput& input) {
    MaxflowCSRGraph csr;
    csr.vertex_count = input.vertex_count;
    csr.source = input.source;
    csr.sink = input.sink;
    csr.row_ptr.assign(static_cast<size_t>(csr.vertex_count) + 1, 0);

    for (const auto& edge : input.edges) {
        if (edge.source >= 0 && edge.source < csr.vertex_count) {
            ++csr.row_ptr[static_cast<size_t>(edge.source) + 1];
        }
    }
    for (int i = 1; i <= csr.vertex_count; ++i) {
        csr.row_ptr[i] += csr.row_ptr[i - 1];
    }

    const size_t edge_count = static_cast<size_t>(csr.row_ptr[csr.vertex_count]);
    csr.col_index.assign(edge_count, 0);
    csr.capacities.assign(edge_count, 0);

    std::vector<int> next_offset = csr.row_ptr;
    for (const auto& edge : input.edges) {
        if (edge.source < 0 || edge.source >= csr.vertex_count) continue;
        const int idx = next_offset[edge.source]++;
        csr.col_index[static_cast<size_t>(idx)] = edge.destination;
        csr.capacities[static_cast<size_t>(idx)] = edge.capacity;
    }

    return csr;
}

}

MaxflowAdjacencyInput MaxflowMincutSolver::parse_input(const std::string& filepath) {
    return parseAdjacencyInput(filepath);
}

MaxflowCSRGraph MaxflowMincutSolver::convert_to_csr(const MaxflowAdjacencyInput& input) {
    return convertToCSR(input);
}

void MaxflowMincutSolver::solve(const MaxflowCSRGraph& csr_graph) {
    Dinic d(csr_graph.vertex_count, csr_graph.source, csr_graph.sink);
    for (int u = 0; u < csr_graph.vertex_count; ++u) {
        for (int idx = csr_graph.row_ptr[u]; idx < csr_graph.row_ptr[u + 1]; ++idx) {
            d.addEdge(u, csr_graph.col_index[static_cast<size_t>(idx)],
                      csr_graph.capacities[static_cast<size_t>(idx)]);
        }
    }

    auto start = std::chrono::high_resolution_clock::now();
    long long maxflow = d.maxFlow();
    std::vector<int> source_reachable;
    long long mincut = 0;
    std::vector<CutEdge> cut_edges;
    d.computeCutData(source_reachable, mincut, cut_edges);

    auto end = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1000000.0;

    std::cout << "Algorithm: Maxflow-Mincut\n";
    std::cout << "Source: " << csr_graph.source << "\n";
    std::cout << "Sink: " << csr_graph.sink << "\n";
    std::cout << "Maximum flow: " << maxflow << "\n";
    std::cout << "Minimum cut capacity: " << mincut << "\n";

    std::cout << "Source side:";
    for (int v = 0; v < csr_graph.vertex_count; ++v) {
        if (source_reachable[v]) {
            std::cout << " " << v;
        }
    }
    std::cout << "\n";

    std::cout << "Sink side:";
    for (int v = 0; v < csr_graph.vertex_count; ++v) {
        if (!source_reachable[v]) {
            std::cout << " " << v;
        }
    }
    std::cout << "\n";

    std::cout << "Cut edges:\n";
    for (const auto& edge : cut_edges) {
        std::cout << edge.from << " " << edge.to << " " << edge.capacity << "\n";
    }

    std::cout << "Execution time: " << ms << " ms\n";
}
