/**
 * @file day_5_print_queue.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compile command : g++ -std=c++17 ./day_5_print_queue.cpp
 * @date 2025-01-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <string_view>
#include <algorithm>
#include <utility>
#include <sstream>
#include <unordered_map>

using namespace std;


vector<string> splitString(const string &str, char delimiter) {
    auto tokens      = vector<string>{};
    auto token       = string {};
    auto tokenStream = istringstream(str);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

pair<vector<pair<int, int>>, vector<vector<int>>> ReadInput(string_view filename)
{
    auto fin              = ifstream{ filename.data() };
    auto mappings_vector  = vector<pair<int, int>>{};
    auto orderings_vector = vector<vector<int>>{};

    //For reading orderings
    for (auto line = string{}; getline(fin, line) && !line.empty();)
    {
        auto pipe_pos = line.find('|');
        auto first    = stoi(line.substr(0, pipe_pos));
        auto second   = stoi(line.substr(pipe_pos + 1));
        mappings_vector.push_back({ first, second });
    }
    //For reading sequence
    for (auto line = string{}; getline(fin, line) && !line.empty();)
    {
        auto orderings = vector<int>{};
        for (auto &token : splitString(line, ','))
        {
            orderings.push_back(stoi(token));
        }
        orderings_vector.push_back(orderings);
    }
    return { mappings_vector, orderings_vector };
}

unordered_map<int, vector<int>> CreateGraph(const vector<pair<int, int>> &mappings)
{
    auto graph = unordered_map<int, vector<int>>{};
    for (const auto &[from, to] : mappings)
    {
        graph[from].push_back(to);
    }
    return graph;
}


bool IsValidOrdering(const unordered_map<int, vector<int>> &kMapping, const vector<int> &kOrdering)
{
    auto is_valid = true;
    for (auto iter = cbegin(kOrdering); is_valid && iter != cend(kOrdering); ++iter)
    {
        if (kMapping.find(*iter) != cend(kMapping))
        {
            const auto kPageOrder = kMapping.at(*iter);
            for (const auto kElem : kPageOrder)
            {
                auto it = find(cbegin(kOrdering), iter, kElem);
                if (it != iter)
                {
                    is_valid = false;
                    break;
                }
            }
        }
    }
    return is_valid;
};

long Day5Part1(const unordered_map<int, vector<int>> &graph, const vector<vector<int>> &orderings)
{
    auto sum = 0;
    for (const auto &kOrdering : orderings)
    {
        if (IsValidOrdering(graph, kOrdering))
        {
            sum += kOrdering.at(size(kOrdering) / 2);
        }
    }

    return sum;
}

long Day5Part2(const unordered_map<int, vector<int>> &graph, const vector<vector<int>> &orderings)
{
    auto sum = 0;
    for (auto ordering : orderings)
    {
        if (false == IsValidOrdering(graph, ordering))
        {
            nth_element(begin(ordering), begin(ordering) + size(ordering) / 2, end(ordering), [&graph](const int &a, const int &b){
                if (graph.find(a) != cend(graph))
                {
                    if (const auto kVec1 = graph.at(a); find(cbegin(kVec1), cend(kVec1), b) != cend(kVec1))
                    {
                        return true;
                    }
                }
                return false;
            });
            sum += ordering.at(size(ordering) / 2);
        }
    }
    return sum;
}

int main(int argc, const char *args[])
{
    const auto [mappings, orderings] = ReadInput(args[1]);
    const auto kMappingsUmap         = CreateGraph(mappings);
    cout << "Day 5, Part 1: " << Day5Part1(kMappingsUmap, orderings) << endl;
    cout << "Day 5, Part 2: " << Day5Part2(kMappingsUmap, orderings) << endl;
    return 0;
}

