/**
 * @file day_18_ram_run.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation: g++ -std=c++17 day_18_ram_run.cpp
 * @date 2025-04-01
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string_view>
#include <algorithm>
#include <iterator>
#include <utility>
#include <queue>
#include <array>

using std::array;
using std::cout;
using std::endl;
using std::for_each_n;
using std::ifstream;
using std::istream_iterator;
using std::min;
using std::max;
using std::pair;
using std::queue;
using std::size;
using std::string;
using std::string_view;
using std::transform;
using std::vector;

struct GridIndex
{
    int row_idx;
    int col_idx;
    bool operator==(const GridIndex& other) const
    {
        return row_idx == other.row_idx && col_idx == other.col_idx;
    }
};

vector<GridIndex> ReadInput(string_view filename)
{
    auto positions = vector<GridIndex>{};
    if (auto fin = ifstream{ filename.data() }; fin)
    {
        transform(istream_iterator<string>{ fin }, istream_iterator<string>{ }, back_inserter(positions),
        [](string_view line) {
            auto comma_pos = line.find(',');
            auto pos       = GridIndex{};
            pos.col_idx    = stoi(string(line.substr(0, comma_pos)));
            pos.row_idx    = stoi(string(line.substr(comma_pos + 1)));
            return pos;
        });   
    }
    return positions;
}

pair<int, int> CalculateMaxDimensions(const vector<GridIndex>& positions, const size_t &n)
{
    auto max_row = 0;
    auto max_col = 0;
    for_each_n(cbegin(positions), n, 
    [&](const auto& first) {
        const auto& [kRowIdx, kColIdx] = first;
        max_row = max(max_row, kRowIdx);
        max_col = max(max_col, kColIdx);
    });
    return {max_row + 1, max_col + 1};
}


vector<vector<char>> CreateGrid(const size_t &rows, const size_t& cols)
{
    auto grid = vector<vector<char>>(rows, vector<char>(cols, '.'));
    return grid;
}

void MarkPositionsInGrid(const vector<GridIndex>& positions, const size_t &n, vector<vector<char>>& grid)
{
    for_each_n(cbegin(positions), n, [&grid](const auto& pos) {
        const auto& [kRowIdx, kColIdx] = pos;
        grid[kRowIdx][kColIdx] = '#';
    });
}

bool IsValueInRange(const int &value, const int &min, const int &max)
{
    return value >= min && value < max;
}

struct Node
{
    GridIndex pos;
    int cost;
};

int FindMinimumDistancesBFS(const vector<vector<char>>& grid, const GridIndex& start, const GridIndex& end)
{
    auto min_cost = -1;
    auto visited  = vector<vector<bool>>(size(grid), vector<bool>(size(grid[0]), false));
    auto queue    = std::queue<Node>{};
    queue.push({start, 0});
    visited[start.row_idx][start.col_idx] = true;

    while (!queue.empty())
    {
        const auto &[kPos, kCost] = queue.front();
        queue.pop();

        if (kPos == end)
        {
            min_cost = kCost;
            break;
        }

        constexpr auto kDirectionsOffsets = std::array<std::pair<int, int>, 4>{{
            {-1, 0}, // Up
            {1, 0},  // Down
            {0, -1}, // Left
            {0, 1}   // Right
        }};
        for (const auto &[dx, dy] : kDirectionsOffsets)
        {
            const auto new_row = kPos.row_idx + dx;
            const auto new_col = kPos.col_idx + dy;

            if (IsValueInRange(new_row, 0, grid.size())
                && IsValueInRange(new_col, 0, grid[0].size())
                && !visited[new_row][new_col]
                && '#' != grid[new_row][new_col]
            )
            {
                visited[new_row][new_col] = true;
                queue.push({{new_row, new_col}, kCost + 1});
            }
        }

    }
    return min_cost;
}


int Day18Part1(const vector<GridIndex>& kBytesPositions, const size_t &n)
{
    const auto [kRowCount, kColumnCount] = CalculateMaxDimensions(kBytesPositions, n);
    auto grid                            = CreateGrid(kRowCount, kColumnCount);
    MarkPositionsInGrid(kBytesPositions, n, grid);
    return FindMinimumDistancesBFS(grid, {0, 0}, {kRowCount - 1, kColumnCount - 1});
}

string Day18Part2(const vector<GridIndex> &kBytePositions)
{
    const auto [kRowCount, kColumnCount] = CalculateMaxDimensions(kBytePositions, size(kBytePositions));
    auto grid                            = CreateGrid(kRowCount, kColumnCount);
    auto str                             = string{};
    for (const auto& pos : kBytePositions)
    {
        grid[pos.row_idx][pos.col_idx] = '#';
        if (FindMinimumDistancesBFS(grid, { 0, 0 }, { kRowCount - 1, kColumnCount - 1 }) == -1)
        {
            str = std::to_string(pos.col_idx).append(",").append(std::to_string(pos.row_idx));
            break;
        }
    }
    return str;
}

int main(int argc, const char *args[])
{
    if (argc != 3)
    {
        cout << "Usage: " << args[0] << " <input_file> <n>" << endl;
        return 0;
    }
    const auto kBytesPositions = ReadInput(args[1]);
    cout << "Day 18, Part 1: " << Day18Part1(kBytesPositions, std::stoi(args[2])) << endl;
    cout << "Day 18, Part 2: " << Day18Part2(kBytesPositions) << endl;
    
    return 0;
}