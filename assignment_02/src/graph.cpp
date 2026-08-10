#include "../include/graph.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

bool read_undirected_graph(const std::string& str_filename, GraphInput& struct_input) {
    std::ifstream stream_fin(str_filename);
    if (!stream_fin.is_open()) {
        std::string str_alt = "tests/" + str_filename;
        stream_fin.open(str_alt);
    }
    if (!stream_fin.is_open()) {
        std::cerr << "Error: Cannot open file: " << str_filename << "\n";
        return false;
    }

    if (!(stream_fin >> struct_input.i_vertex_count >> struct_input.i_edge_count)) {
        std::cerr << "Error: Failed to read vertex or edge count.\n";
        return false;
    }

    if (struct_input.i_vertex_count <= 0) {
        std::cerr << "Error: Invalid number of vertices: " << struct_input.i_vertex_count << "\n";
        return false;
    }

    struct_input.v_adj_list.assign(struct_input.i_vertex_count, std::vector<Edge>());

    std::string str_line;
    // Consume remainder of first line
    std::getline(stream_fin, str_line);

    int parsed_vertices = 0;
    while (parsed_vertices < struct_input.i_vertex_count && std::getline(stream_fin, str_line)) {
        if (str_line.empty()) {
            continue;
        }

        std::stringstream ss(str_line);
        int u = -1;
        int degree = -1;
        if (!(ss >> u >> degree)) {
            continue; // Skip lines with no vertex/degree data
        }

        if (u < 0 || u >= struct_input.i_vertex_count) {
            std::cerr << "Error: Vertex ID out of range: " << u << "\n";
            return false;
        }

        if (degree < 0) {
            std::cerr << "Error: Degree cannot be negative for vertex: " << u << "\n";
            return false;
        }

        std::vector<int> values;
        int val;
        while (ss >> val) {
            values.push_back(val);
        }

        bool is_weighted = false;
        if (values.size() == static_cast<size_t>(degree)) {
            is_weighted = false;
        } else if (values.size() == static_cast<size_t>(2 * degree)) {
            is_weighted = true;
        } else {
            std::cerr << "Error: Vertex " << u << " has degree " << degree 
                      << " but we found " << values.size() << " elements on the line.\n";
            return false;
        }

        for (int i = 0; i < degree; ++i) {
            int dest = -1;
            int weight = 1;

            if (is_weighted) {
                dest = values[2 * i];
                weight = values[2 * i + 1];
            } else {
                dest = values[i];
                weight = 1;
            }

            if (dest < 0 || dest >= struct_input.i_vertex_count) {
                std::cerr << "Error: Neighbor out of range: " << dest << " for vertex: " << u << "\n";
                return false;
            }

            if (weight < 0) {
                std::cerr << "Error: Rejecting negative weight " << weight << " on undirected edge (" << u << ", " << dest << ").\n";
                return false;
            }

            Edge edge;
            edge.i_destination = dest;
            edge.i_weight = weight;
            struct_input.v_adj_list[u].push_back(edge);
        }

        parsed_vertices++;
    }

    // Sort neighbor lists for each vertex in ascending order of destination
    for (int i = 0; i < struct_input.i_vertex_count; ++i) {
        std::sort(struct_input.v_adj_list[i].begin(), struct_input.v_adj_list[i].end(), [](const Edge& a, const Edge& b) {
            return a.i_destination < b.i_destination;
        });
    }

    return true;
}
