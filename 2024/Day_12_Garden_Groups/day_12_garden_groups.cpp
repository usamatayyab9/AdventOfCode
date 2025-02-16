#include <vector>
#include <iostream>
#include <fstream>
#include <string_view>
#include <algorithm>
#include <string>
#include <iterator>
#include <array>
#include <numeric>
#include <cmath>

using std::array;
using std::back_inserter;
using std::cout;
using std::endl;
using std::istream_iterator;
using std::size;
using std::sqrt;
using std::string;
using std::string_view;
using std::transform;
using std::ifstream;
using std::vector;

vector<vector<char>> ReadInput(string_view filename)
{
    auto grid = vector<vector<char>>{};
    auto fin  = ifstream{ filename.data() };
    transform(istream_iterator<string>{ fin }, istream_iterator<string>{}, back_inserter(grid), [](const auto line) -> vector<char>
    {
        return { cbegin(line), cend(line) };
    });
    return grid;
}

struct Index
{
    int r_idx;
    int col_idx;
    bool operator<(const Index &other) const
    {
        return r_idx < other.r_idx || (r_idx == other.r_idx && col_idx < other.col_idx);
    }

    bool operator==(const Index &other) const
    {
        return r_idx == other.r_idx && col_idx == other.col_idx;
    }
};

template <class T>
bool Contains(const vector<T> &container, const T &value)
{
    return find(cbegin(container), cend(container), value) != cend(container);
}

using Region = vector<Index>;

bool IsValueInRange(const int &value, const int &lo, const int &hi)
{
    return value >= lo && value < hi;
}

Region ExploreRegion(const vector<vector<char>> &kGrid, int row_idx, int col_idx, vector<vector<bool>> &visited_cells)
{
    constexpr auto kOffsets = array<Index, 4>{{
                { -1, 0 },
        { 0, -1 },      { 0, +1 }, 
                { +1, 0 } 
    }};
    auto region                     = Region{ Index{ row_idx, col_idx } };
    const auto &kCurrentPlant       = kGrid[row_idx][col_idx];
    visited_cells[row_idx][col_idx] = true;

    for (const auto &[kRowOffset, kColOffset] : kOffsets)
    {
        const auto kNewRowIdx = row_idx + kRowOffset;
        const auto kNewColIdx = col_idx + kColOffset;
        if (IsValueInRange(kNewRowIdx, 0, size(visited_cells))             &&
            IsValueInRange(kNewColIdx, 0, size(visited_cells[kNewRowIdx])) &&
            !visited_cells[kNewRowIdx][kNewColIdx]                         &&
            kCurrentPlant == kGrid[kNewRowIdx][kNewColIdx]
        )
        {
            const auto kConnectedRegion = ExploreRegion(kGrid, kNewRowIdx, kNewColIdx, visited_cells);
            region.insert(cend(region), cbegin(kConnectedRegion), cend(kConnectedRegion));
        }
    }
    return region;
}

size_t CalculatePerimeterOfRegion(const Region &region, const vector<vector<char>> &kGrid)
{
    constexpr auto kOffsets = array<Index, 4>{{ { -1, 0 }, { 0, +1 }, { +1, 0 }, { 0, -1 } }};
    auto perimeter = size_t{ 0 };
    for (const auto &[kRowIdx, kColIdx] : region)
    {
        const auto &kCurrentPlant = kGrid[kRowIdx][kColIdx];
        if ((0 == kRowIdx)               || (kCurrentPlant != kGrid[kRowIdx - 1][kColIdx    ])) { ++perimeter; }
        if ((size(kGrid) - 1 == kColIdx) || (kCurrentPlant != kGrid[kRowIdx    ][kColIdx + 1])) { ++perimeter; }
        if ((size(kGrid) - 1 == kRowIdx) || (kCurrentPlant != kGrid[kRowIdx + 1][kColIdx    ])) { ++perimeter; }
        if ((0 == kColIdx)               || (kCurrentPlant != kGrid[kRowIdx    ][kColIdx - 1])) { ++perimeter; }
    }
    return perimeter;
}

vector<Region> FindRegions(const vector<vector<char>> &kGrid)
{
    auto visited_cells = vector<vector<bool>>(size(kGrid), vector<bool>(size(kGrid), false));
    auto regions      = vector<Region>{};
    
    for (auto ii = 0; ii < size(visited_cells) ;++ii)
    {
        for (auto jj = 0; jj < size(visited_cells[ii]) ;++jj)
        {
            if (false == visited_cells[ii][jj])
            {
                regions.push_back(ExploreRegion(kGrid, ii, jj, visited_cells));
            }
        }
    }

    return regions;
}

size_t Day12Part1(const vector<vector<char>> &kGrid)
{
    const auto kAllRegions = FindRegions(kGrid);
    const auto kPrice      = std::accumulate(cbegin(kAllRegions), cend(kAllRegions), size_t{ 0 },
    [&kGrid](const auto init, const Region &region)
    {
        return init + size(region) * CalculatePerimeterOfRegion(region, kGrid);
    });

    return kPrice;
}

size_t GetPerimeterPoints(const Region &region)
{
    auto corners_count = size_t{ 0 };
    for (const auto &[x, y] : region)
    {
        if (!Contains(region, Index{ x - 1, y}) && !Contains(region, Index{ x, y - 1})) { ++corners_count; }
        if (!Contains(region, Index{ x + 1, y}) && !Contains(region, Index{ x, y - 1})) { ++corners_count; }
        if (!Contains(region, Index{ x - 1, y}) && !Contains(region, Index{ x, y + 1})) { ++corners_count; }
        if (!Contains(region, Index{ x + 1, y}) && !Contains(region, Index{ x, y + 1})) { ++corners_count; }

        if (Contains(region, Index{ x - 1, y}) && Contains(region, Index{ x, y - 1}) && !Contains(region, Index{ x - 1, y - 1})) { ++corners_count; }
        if (Contains(region, Index{ x + 1, y}) && Contains(region, Index{ x, y - 1}) && !Contains(region, Index{ x + 1, y - 1})) { ++corners_count; }
        if (Contains(region, Index{ x - 1, y}) && Contains(region, Index{ x, y + 1}) && !Contains(region, Index{ x - 1, y + 1})) { ++corners_count; }
        if (Contains(region, Index{ x + 1, y}) && Contains(region, Index{ x, y + 1}) && !Contains(region, Index{ x + 1, y + 1})) { ++corners_count; }

    }
    return corners_count;
}

size_t Day12Part2(const vector<vector<char>> &kGrid)
{
    const auto kAllRegions = FindRegions(kGrid);
    auto total_price       = size_t{ 0 };
    auto idx               = 0;
    for (const auto &kRegion : kAllRegions)
    {
        cout << "Region: " << kGrid[kRegion[0].r_idx][kRegion[0].col_idx] << ", Region# " << idx << endl;
        for (const auto &[kRowIdx, kColIdx] : kRegion)
        {
            cout << "(" << kRowIdx << ", " << kColIdx << ") " << endl;
        }
        total_price += size(kRegion) * GetPerimeterPoints(kRegion);
        cout << "Perimeter: " << GetPerimeterPoints(kRegion) << endl;
        cout << endl;
        ++idx;
    }
    return total_price;
}

int main(int argc, const char *args[])
{
    const auto kGrid = ReadInput(args[1]);
    cout << "Day 12, Part 1: " << Day12Part1(kGrid) << endl;
    cout << "Day 12, Part 2: \n" << Day12Part2(kGrid) << endl;
    return 0;
}