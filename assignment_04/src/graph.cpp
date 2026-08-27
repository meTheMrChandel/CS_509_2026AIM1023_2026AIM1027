#include "../include/graph.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

// Helper to open the file
static std::ifstream open_file(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::string alt = "tests/" + filename;
        fin.open(alt);
    }
    return fin;
}

bool read_vertex_coloring_graph(const std::string& filename, int& V, int& E, AdjList& adj) {
    std::ifstream fin = open_file(filename);
    if (!fin.is_open()) {
        std::cerr << "Error: Unable to open file '" << filename << "'\n";
        return false;
    }

    if (!(fin >> V >> E)) {
        std::cerr << "Error: Failed to read V and E.\n";
        return false;
    }

    if (V <= 0) {
        std::cerr << "Error: Invalid number of vertices (V = " << V << "). Must be positive.\n";
        return false;
    }
    if (E < 0) {
        std::cerr << "Error: Invalid number of edges (E = " << E << "). Cannot be negative.\n";
        return false;
    }

    adj.assign(V, std::vector<int>());

    std::string line;
    // Consume remainder of the first line
    std::getline(fin, line);

    int vertices_parsed = 0;
    while (vertices_parsed < V && std::getline(fin, line)) {
        // Skip empty lines
        if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }

        std::stringstream ss(line);
        int u, degree;
        if (!(ss >> u >> degree)) {
            std::cerr << "Error: Invalid line format for vertex list.\n";
            return false;
        }

        if (u < 0 || u >= V) {
            std::cerr << "Error: Vertex ID out of range (u = " << u << ", V = " << V << ").\n";
            return false;
        }

        if (degree < 0) {
            std::cerr << "Error: Negative degree (" << degree << ") for vertex " << u << ".\n";
            return false;
        }

        std::vector<int> neighbors;
        int neighbor;
        while (ss >> neighbor) {
            neighbors.push_back(neighbor);
        }

        if (neighbors.size() != static_cast<size_t>(degree)) {
            std::cerr << "Error: Mismatched degree/neighbour count for vertex " << u 
                      << " (expected " << degree << ", found " << neighbors.size() << ").\n";
            return false;
        }

        for (int n : neighbors) {
            if (n < 0 || n >= V) {
                std::cerr << "Error: Neighbor ID out of range: " << n << " for vertex " << u << ".\n";
                return false;
            }
            if (n == u) {
                std::cerr << "Error: Self-loop detected on vertex " << u << ".\n";
                return false;
            }
        }

        adj[u] = neighbors;
        vertices_parsed++;
    }

    if (vertices_parsed < V) {
        std::cerr << "Error: Adjacency list contains only " << vertices_parsed << " vertices, expected " << V << ".\n";
        return false;
    }

    // Verify undirected (symmetric) property
    for (int u = 0; u < V; ++u) {
        for (int n : adj[u]) {
            // n must have u in its list
            auto it = std::find(adj[n].begin(), adj[n].end(), u);
            if (it == adj[n].end()) {
                std::cerr << "Error: Graph is not undirected. Edge between " << u << " and " << n << " is asymmetric.\n";
                return false;
            }
        }
    }

    fin.close();
    return true;
}

bool read_pagerank_graph(const std::string& filename, int& V, int& E, AdjList& adj, PageRankParams& params) {
    std::ifstream fin = open_file(filename);
    if (!fin.is_open()) {
        std::cerr << "Error: Unable to open file '" << filename << "'\n";
        return false;
    }

    if (!(fin >> V >> E)) {
        std::cerr << "Error: Failed to read V and E.\n";
        return false;
    }

    if (V <= 0) {
        std::cerr << "Error: Invalid number of vertices (V = " << V << ").\n";
        return false;
    }
    if (E < 0) {
        std::cerr << "Error: Invalid number of edges (E = " << E << ").\n";
        return false;
    }

    adj.assign(V, std::vector<int>());

    std::string line;
    // Consume remainder of the first line
    std::getline(fin, line);

    int vertices_parsed = 0;
    while (vertices_parsed < V && std::getline(fin, line)) {
        if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }

        std::stringstream ss(line);
        int u, out_degree;
        if (!(ss >> u >> out_degree)) {
            std::cerr << "Error: Invalid line format for vertex list in PageRank.\n";
            return false;
        }

        if (u < 0 || u >= V) {
            std::cerr << "Error: Vertex ID out of range (u = " << u << ", V = " << V << ") in PageRank.\n";
            return false;
        }

        if (out_degree < 0) {
            std::cerr << "Error: Negative out-degree (" << out_degree << ") for vertex " << u << ".\n";
            return false;
        }

        std::vector<int> neighbors;
        int neighbor;
        while (ss >> neighbor) {
            neighbors.push_back(neighbor);
        }

        if (neighbors.size() != static_cast<size_t>(out_degree)) {
            std::cerr << "Error: Mismatched outdegree/neighbour count for vertex " << u 
                      << " (expected " << out_degree << ", found " << neighbors.size() << ").\n";
            return false;
        }

        for (int n : neighbors) {
            if (n < 0 || n >= V) {
                std::cerr << "Error: Neighbor ID out of range: " << n << " for vertex " << u << ".\n";
                return false;
            }
        }

        adj[u] = neighbors;
        vertices_parsed++;
    }

    if (vertices_parsed < V) {
        std::cerr << "Error: Adjacency list contains only " << vertices_parsed << " vertices, expected " << V << " in PageRank.\n";
        return false;
    }

    // Now read params: DAMPING, TOLERANCE, MAX_ITERATIONS
    std::string key;
    bool has_damping = false;
    bool has_tolerance = false;
    bool has_max_iter = false;

    while (fin >> key) {
        if (key == "DAMPING") {
            if (!(fin >> params.damping)) {
                std::cerr << "Error: Failed to parse DAMPING value.\n";
                return false;
            }
            has_damping = true;
        } else if (key == "TOLERANCE") {
            if (!(fin >> params.tolerance)) {
                std::cerr << "Error: Failed to parse TOLERANCE value.\n";
                return false;
            }
            has_tolerance = true;
        } else if (key == "MAX_ITERATIONS") {
            if (!(fin >> params.max_iterations)) {
                std::cerr << "Error: Failed to parse MAX_ITERATIONS value.\n";
                return false;
            }
            has_max_iter = true;
        }
    }

    // Validate parameters
    if (has_damping) {
        if (params.damping <= 0.0 || params.damping >= 1.0) {
            std::cerr << "Error: Damping factor must be strictly between 0 and 1 (found " << params.damping << ").\n";
            return false;
        }
    } else {
        std::cerr << "Error: DAMPING parameter is missing.\n";
        return false;
    }

    if (has_tolerance) {
        if (params.tolerance <= 0.0) {
            std::cerr << "Error: Tolerance must be positive (found " << params.tolerance << ").\n";
            return false;
        }
    } else {
        std::cerr << "Error: TOLERANCE parameter is missing.\n";
        return false;
    }

    if (has_max_iter) {
        if (params.max_iterations <= 0) {
            std::cerr << "Error: MAX_ITERATIONS must be positive (found " << params.max_iterations << ").\n";
            return false;
        }
    } else {
        std::cerr << "Error: MAX_ITERATIONS parameter is missing.\n";
        return false;
    }

    fin.close();
    return true;
}
