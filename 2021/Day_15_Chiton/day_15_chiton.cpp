/**
 * @file day_15_chiton.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief
 * Compilation command : g++ -std=c++17 ./day_15_chiton.cpp
 * Problem statement is aviailable at : https://adventofcode.com/2021/day/15
 * 
 * Solution:
 * For solving this problem I used dijkstra algorithm for finding the path
 * with minimum cost.
 * - `IsValInRange()` Function:
 *      This function checks if a given value lies within a specified range.
 * 
 * - `Grid` Class:
 *      Represents a 2D grid. It provides functionality to access elements, get the number of rows and columns,
 *      and add rows to the grid.
 * 
 * - `ScaledRepeatedGrid` Class:
 *      This class represents a grid that repeats its content in a scaled manner. It takes a base grid and a
 *      scaling factor. When accessing elements, if an index exceeds the dimensions of the base grid, it wraps
 *      around to repeat the content.
 * 
 * - `ReadInput()` Function:
 *      Reads input from a file and constructs a grid based on the contents of the file.
 * 
 * - `MinimumCostPath()` Function:
 *      Finds the minimum cost path through the grid. It uses Dijkstra's algorithm with a priority queue.
 *      It maintains a queue of elements representing positions in the grid along with their costs. It iteratively
 *      explores neighboring cells, updating the cost and adding them to the queue if necessary. The algorithm
 *      terminates when it reaches the bottom-right cell of the grid.
 * 
 * See function comments for more dtails.
 * 
 * Driver code :
 * This program expects one additional argument via command line. This argument is treated as an input file path.
 * Read the data from a file in a grid object.
 * Construct a scaled grid object from the above grid object(Used for Part 2).
 * Use `MinimumCostPath()` for both normal and scaled grid to computed minimum cost.
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <iterator>
#include <queue>
#include <set>
#include <array>
#include <exception>

using std::array;
using std::back_inserter;
using std::cout;
using std::cbegin;
using std::cend;
using std::string;
using std::string_view;
using std::ifstream;
using std::istream_iterator;
using std::transform;
using std::pair;
using std::vector;
using std::priority_queue;
using std::set;

/**
 * @brief  Function to check if a value is within a specified range
 * @param value 
 * @param min 
 * @param max 
 * @return true 
 * @return false 
 */
bool IsValInRange(const auto &value, const auto &min, const auto &max)
{
    return value >= min && value < max;
}

/**
 * @brief Class representing a 2D grid
 * 
 */
class Grid
{
public:
    Grid() = default;

    /**
     * @brief Accessor for reading elements (const version)
     * @param i - Row index
     * @param j - Columne Index
     * @return const int& 
     */
    const int & operator()(const size_t &i, const size_t &j) const
    {
        return data.at(i).at(j);
    }
    int& operator()(const size_t &i, const size_t &j)
    {
        return data.at(i).at(j);
    }
    size_t RowCount() const { return size(data); }
    size_t ColumnCount() const { return 0 == size(data) ? 0 : size(data[0]); }
    void Pushback(const vector<int> &vec)
    {
        data.push_back(vec);
    }
private:
    vector<vector<int>> data;
};

/**
 * @brief Class representing a grid with repeated content, scaled by a factor
 * 
 */
class ScaledRepeatedGrid
{
    public :
    ScaledRepeatedGrid(const Grid &grid_arg, const int &scaled_factor) :
        single_tile{ grid_arg },
        scale_factor { scaled_factor }
    {
    }

    /**
     * @brief Returns cell value in a scaled grid
     * 
     * @param i 
     * @param j 
     * @return const int 
     */
    const int operator()(const size_t &i, const size_t &j) const
    {
        if (i >= RowCount() || j >= ColumnCount())
        {
            throw std::out_of_range{ "Out of range index accessed" };
        }
        const auto kOriginalRowIdx  = i % single_tile.RowCount();
        const auto kOriginalColIdx  = j % single_tile.ColumnCount();
        const auto kRowShiftFactor  = i / single_tile.RowCount();
        const auto kColShiftFactor  = j / single_tile.ColumnCount();
        const auto kOriginalElement = single_tile(kOriginalRowIdx, kOriginalColIdx);
        auto       shifted_value    = kOriginalElement + kRowShiftFactor + kColShiftFactor;
        if (shifted_value > 9)
        {
            shifted_value -= 9; 
        }
        return shifted_value;
    }

    size_t RowCount() const { return single_tile.RowCount() * scale_factor; }
    size_t ColumnCount() const { return 0 == single_tile.RowCount() ? 0 : single_tile.ColumnCount() * scale_factor; }

    private:
    int scale_factor{ 1 };
    Grid single_tile;
};

/**
 * @brief Function to read input from a file and construct a grid
 * @param filename 
 * @return Grid object
 */
Grid ReadInput(string_view filename)
{
    auto fin  = ifstream{ filename.data() };
    auto grid = Grid{}; 
    for_each(istream_iterator<string>{ fin }, istream_iterator<string>{},
    [&grid](const string &str)
    {
        auto vec = vector<int>(size(str), 0);
        transform(cbegin(str), cend(str), begin(vec), [](const char &ch){ return ch - '0'; });
        grid.Pushback(vec);
    });

    return grid;
}

/**
 * @brief Computes the minimum cost of traversing from top left of the grid to
 * bottom right. Uses Dijkstra algorithm to find minimum cost.
 * 
 * @tparam GridType 
 * @param grid 
 * @return size_t 
 */
template<class GridType>
size_t MinimumCostPath(const GridType &grid)
{
    struct PriorityQueuElement
    {
        size_t cost_so_far{ 0 };
        long long row_idx;
        long long col_idx;
        bool operator<(const PriorityQueuElement &other) const
        {
            return other.cost_so_far < cost_so_far;
        }
    };
    constexpr auto kDirectionsOffsets = array<pair<int, int>, 4>{{
                 { -1, 0 },
        { 0, -1 },        { 0, +1 },
                { +1, 0 }
    }};
    auto minimum_costs_queue = priority_queue<PriorityQueuElement>{ };// Heap for retreiving lowest cost indices
    auto total_minum_cost    = size_t{ 0 };
    minimum_costs_queue.push(PriorityQueuElement{ 0, 0, 0 }); //Initial cost of visiting (0,0) is 0 as we are already in that cell
    vector<vector<long long>> dist(grid.RowCount(), vector<long long>(grid.ColumnCount(), std::numeric_limits<long long>::max()));
    dist[0][0] = 0;
    while (false == minimum_costs_queue.empty())
    {
        const auto [minimum_cost_so_far, row_idx, col_idx] = minimum_costs_queue.top();
        minimum_costs_queue.pop();
        if (row_idx == grid.RowCount() - 1 && col_idx == grid.ColumnCount() - 1)
        {
            //coming here would mean we have reached the bottom right cell
            total_minum_cost = minimum_cost_so_far;
            break;           
        }
        for (const auto &[kRowOffset, kColumnOffset] : kDirectionsOffsets)
        {
            const auto kRowIdxToProcess = row_idx + kRowOffset;
            const auto kColIdxToProcess = col_idx + kColumnOffset;
            if (IsValInRange(kRowIdxToProcess, 0, grid.RowCount()) &&
                IsValInRange(kColIdxToProcess, 0, grid.ColumnCount()) &&
                dist[row_idx][col_idx] + grid(kRowIdxToProcess, kColIdxToProcess) < dist[kRowIdxToProcess][kColIdxToProcess]
            )
            {
                dist[kRowIdxToProcess][kColIdxToProcess] = dist[row_idx][col_idx] + grid(kRowIdxToProcess, kColIdxToProcess);
                minimum_costs_queue.push(
                    PriorityQueuElement{ static_cast<size_t>(grid(kRowIdxToProcess, kColIdxToProcess)) + minimum_cost_so_far,
                    kRowIdxToProcess, kColIdxToProcess } 
                );
            }
        }
    }
    return total_minum_cost;
}

int main(int argc, const char *args[])
{
    const auto kGrid       = ReadInput(args[1]);
    const auto kScaledGrid = ScaledRepeatedGrid{ kGrid, 5 };
    cout << "Part 1 : " << MinimumCostPath(kGrid) << "\n";
    cout << "Part 2 : " << MinimumCostPath(kScaledGrid) << "\n";
    return 0;
}