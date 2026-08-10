#include "../include/graph.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// Helper to open the graph file from current path or fall back to tests/ folder.
static std::ifstream open_graph_file(const std::string& str_filename)
{
    std::ifstream stream_fin(str_filename);

    if (!stream_fin.is_open())
    {
        std::string str_alt_filename = "tests/" + str_filename;
        stream_fin.open(str_alt_filename);
    }

    return stream_fin;
}

// Helper to parse the SOURCE keyword and source node ID.
static bool read_source_line(std::ifstream& stream_fin, int& i_source)
{
    std::string str_token;

    while (stream_fin >> str_token)
    {
        if (str_token == "SOURCE")
        {
            if (!(stream_fin >> i_source))
            {
                std::cerr << "Error: Missing source vertex after SOURCE keyword.\n";
                return false;
            }
            return true;
        }
    }

    std::cerr << "Error: SOURCE line not found in input file.\n";
    return false;
}

// Parses unweighted graph data into the input structure.
bool read_unweighted_graph(const std::string& str_filename, GraphInput& struct_input)
{
    std::ifstream stream_fin = open_graph_file(str_filename);

    if (!stream_fin.is_open())
    {
        std::cerr << "Error: Unable to open file " << str_filename << std::endl;
        return false;
    }

    // Read V and E.
    stream_fin >> struct_input.i_vertex_count >> struct_input.i_edge_count;

    if (struct_input.i_vertex_count <= 0)
    {
        std::cerr << "Error: Invalid number of vertices.\n";
        return false;
    }

    // Size adjacency list array buffer.
    struct_input.v_adj_list.assign(struct_input.i_vertex_count, std::vector<Edge>());

    for (int i_v = 0; i_v < struct_input.i_vertex_count; ++i_v)
    {
        int i_vertex_id = 0;
        int i_degree = 0;

        if (!(stream_fin >> i_vertex_id >> i_degree))
        {
            std::cerr << "Error: Invalid adjacency list format at index " << i_v << ".\n";
            return false;
        }

        if (i_vertex_id < 0 || i_vertex_id >= struct_input.i_vertex_count)
        {
            std::cerr << "Error: Vertex ID out of range: " << i_vertex_id << ".\n";
            return false;
        }

        for (int i_d = 0; i_d < i_degree; ++i_d)
        {
            int i_neighbour = 0;
            if (!(stream_fin >> i_neighbour))
            {
                std::cerr << "Error: Missing neighbor for vertex " << i_vertex_id << ".\n";
                return false;
            }

            Edge struct_edge;
            struct_edge.i_destination = i_neighbour;
            struct_edge.i_weight = 1; // Unweighted edge default weight.
            struct_input.v_adj_list[i_vertex_id].push_back(struct_edge);
        }
    }

    // Parse SOURCE node configuration.
    if (!read_source_line(stream_fin, struct_input.i_source))
    {
        return false;
    }

    if (struct_input.i_source < 0 || struct_input.i_source >= struct_input.i_vertex_count)
    {
        std::cerr << "Error: Source vertex out of range.\n";
        return false;
    }

    stream_fin.close();
    return true;
}

// Parses weighted graph data into the input structure.
bool read_weighted_graph(const std::string& str_filename, GraphInput& struct_input)
{
    std::ifstream stream_fin = open_graph_file(str_filename);

    if (!stream_fin.is_open())
    {
        std::cerr << "Error: Unable to open file " << str_filename << std::endl;
        return false;
    }

    // Read V and E.
    stream_fin >> struct_input.i_vertex_count >> struct_input.i_edge_count;

    if (struct_input.i_vertex_count <= 0)
    {
        std::cerr << "Error: Invalid number of vertices.\n";
        return false;
    }

    // Size adjacency list array buffer.
    struct_input.v_adj_list.assign(struct_input.i_vertex_count, std::vector<Edge>());

    for (int i_v = 0; i_v < struct_input.i_vertex_count; ++i_v)
    {
        int i_vertex_id = 0;
        int i_degree = 0;

        if (!(stream_fin >> i_vertex_id >> i_degree))
        {
            std::cerr << "Error: Invalid adjacency list format at index " << i_v << ".\n";
            return false;
        }

        if (i_vertex_id < 0 || i_vertex_id >= struct_input.i_vertex_count)
        {
            std::cerr << "Error: Vertex ID out of range: " << i_vertex_id << ".\n";
            return false;
        }

        for (int i_d = 0; i_d < i_degree; ++i_d)
        {
            int i_neighbour = 0;
            int i_weight = 0;

            if (!(stream_fin >> i_neighbour >> i_weight))
            {
                std::cerr << "Error: Missing neighbor/weight for vertex " << i_vertex_id << ".\n";
                return false;
            }

            if (i_weight <= 0)
            {
                std::cerr << "Error: Edge weight must be positive.\n";
                return false;
            }

            Edge struct_edge;
            struct_edge.i_destination = i_neighbour;
            struct_edge.i_weight = i_weight;
            struct_input.v_adj_list[i_vertex_id].push_back(struct_edge);
        }
    }

    // Parse SOURCE node configuration.
    if (!read_source_line(stream_fin, struct_input.i_source))
    {
        return false;
    }

    if (struct_input.i_source < 0 || struct_input.i_source >= struct_input.i_vertex_count)
    {
        std::cerr << "Error: Source vertex out of range.\n";
        return false;
    }

    stream_fin.close();
    return true;
}
