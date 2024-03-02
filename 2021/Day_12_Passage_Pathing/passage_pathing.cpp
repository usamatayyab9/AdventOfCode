/**
 * @file passage_pathing.cpp
 * @author your name (you@domain.com)
 * @brief 
 * Compilation command : g++ -std=c++17 ./passage_pathing.cpp
 * Compiled with gcc 9.4.0
 * Problem statement : https://adventofcode.com/2021/day/12 
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <iterator>
#include <unordered_map>
#include <unordered_set>

using namespace std;

/**
 * @brief  Define a structure for representing an edge between two vertices.
 * 
 */
struct Edge
{
    string v1;//vertex 1
    string v2;//vertex 2
};

using EdgeList = vector<Edge>;
void PrintEdgeList(const EdgeList& edges)
{
    for (const auto& [v1, v2] : edges)
    {
        cout << v1 << "->" << v2 << endl;
    }
}

EdgeList ReadEdgesFromFile(const string& filename)
{
    auto edges = EdgeList{};
    if (auto fin = ifstream{ filename }; fin)
    {
        // Read each line from the file, representing an edge, and transform it into an Edge object.
        transform(istream_iterator<string>{fin}, istream_iterator<string>{}, back_inserter(edges), [](const string& str)
            {
                auto first = cbegin(str);
                auto last  = cend(str);
                auto pos   = find(first, last, '-');
                return Edge{ string{first, pos}, string{pos + 1, last} };
            });
    }
    return edges;
}

struct Graph
{
    Graph() = delete;
    Graph(const EdgeList& edges)
    {
        // Populate the adjacency matrix based on the given list of edges.
        for (const auto& [v1, v2] : edges)
        {
            madjMat[v1].push_back(v2);
            madjMat[v2].push_back(v1);
        }
    }
    void Print() const
    {
        for (const auto& [vertex_u, vertices_connected_from_u] : madjMat)
        {
            cout << vertex_u << "->";
            for (const auto vertex_v : vertices_connected_from_u) { cout << vertex_v << ","; }
            cout << endl;
        }
    }

    size_t TotalPathsPart1()
    {
        auto visisted_small_caves = unordered_set<string>{};
        auto total_paths = size_t{ 0 };
        TotalPathsPart1_REC("start", visisted_small_caves, total_paths);
        return total_paths;
    }

    /**
     * @brief Recursive function to calculate the total number of paths in the graph (Part 1).
     * @param u The current vertex being visited.
     * @param visited_small_vertices Set of visited small vertices.
     * @param total_paths The total number of paths.
     */
    void TotalPathsPart1_REC(const string& u, unordered_set<string>& visited_small_verteices, size_t& total_paths)
    {
        if (u == "end") { ++total_paths; }
        else
        {
            auto is_vertext_small = IsVertexSmall(u);
            if (is_vertext_small && visited_small_verteices.find(u) != cend(visited_small_verteices)) { /*do nothing*/ }
            else
            {
                if (is_vertext_small) { visited_small_verteices.insert(u); }
                for (const auto& v : madjMat[u])
                {
                    TotalPathsPart1_REC(v, visited_small_verteices, total_paths);
                }
                if (is_vertext_small) { visited_small_verteices.erase(u); }
            }
        }
    }

    /**
     * @brief Function to calculate the total number of paths in the graph (Part 2).
     * @return The total number of paths.
     */
    size_t TotalPathsPart2()
    {
        auto total_paths        = size_t{0};
        auto small_caves_counts = unordered_map<string, size_t>{};
        TotalPathsPart2_REC("start", small_caves_counts, total_paths);
        return total_paths;
    }

    /**
     * @brief Function to check if the visiting criteria for small caves are met.
     * @param small_caves_counts The counts of visits to small caves.
     * @return True if the visiting criteria are met, otherwise false.
     */
    bool IsSmallCaveVisitingCriteria(const unordered_map<string, size_t>& small_caves_counts)
    {
        auto no_of_caves_visited_3_or_more_times = 0;
        auto no_of_caves_visisted_2_times        = 0;
        for (const auto& [key_ignore, value] : small_caves_counts)
        {
            no_of_caves_visited_3_or_more_times += (value >= 3);
            no_of_caves_visisted_2_times        += (value == 2);
        }
        return (no_of_caves_visited_3_or_more_times == 0) && (no_of_caves_visisted_2_times <= 1);
    }

    /**
     * @brief Recursive function to calculate the total number of paths in the graph (Part 2).
     * @param u The current vertex being visited.
     * @param small_caves_counts The counts of visits to small caves.
     * @param total_paths The total number of paths.
     */
    void TotalPathsPart2_REC(const string &u, unordered_map<string, size_t> &small_caves_counts, size_t &total_paths)
    {
        if (IsSmallCaveVisitingCriteria(small_caves_counts) == false) {                return; }
        if (u == "end")                                               { ++total_paths; return; }
        auto is_small_vertex = IsVertexSmall(u);
        if (is_small_vertex) { ++small_caves_counts[u]; }
        for (const auto& v : madjMat[u])
        {
            if (v == "start") { continue; }
            TotalPathsPart2_REC(v, small_caves_counts, total_paths);
        }
        if (is_small_vertex) { --small_caves_counts[u]; }
    }

    bool IsVertexSmall(const string& vrtx)
    {
        return islower(vrtx[0]);//all_of(cbegin(vrtx), cend(vrtx), ::islower);
    }
    unordered_map<string, vector<string>> madjMat;
};

int main(int argc, const char* args[])
{
    cout << "filename: " << args[1] << endl;
    auto edges = ReadEdgesFromFile(args[1]);
    // cout << endl << "Edges read from file: " << endl;
    // PrintEdgeList(edges);
    auto graph = Graph{ edges };
    // cout << endl << "Graph: " << endl;
    // graph.Print(); cout << endl;

    auto total_paths_part1 = graph.TotalPathsPart1();
    auto total_paths_part2 = graph.TotalPathsPart2();
    cout << "TOTAL PATHS (PART-1): " << total_paths_part1 << endl;
    cout << "TOTAL PATHS (PART-2): " << total_paths_part2 << endl;
    return 0;
}
