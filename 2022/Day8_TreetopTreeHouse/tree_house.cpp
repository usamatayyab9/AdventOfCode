#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <string_view>
#include <iterator>
#include<numeric>

using namespace std;

void Print(const auto &grid)
{
    for (const auto &row : grid)
    {
        for (const auto &elem : row)
        {
            cout << elem << ' ';
        }
        cout << endl;
    }
}

vector<vector<int>> ReadGridFromFile(string_view filename)
{
    auto fin    = ifstream{ filename.data() };
    auto grid   = vector<vector<int>>{};
    transform(istream_iterator<string>{ fin }, istream_iterator<string>{},
    back_inserter(grid),
    [](const auto &line)
    {
        auto vec = vector<int>(size(line));
        transform(cbegin(line), cend(line), begin(vec),
        [](const auto &ch)
        {
            return ch - '0';
        });
        return vec;
    });
    return grid;
}

void InitializeVisibilty(vector<vector<bool>> &visibility_grid)
{
    auto MarkCompleteRow = [](vector<bool> &vec){
        fill(begin(vec), end(vec), 1);
    };
    //All trees on borders are visible
    //Mark top row
    MarkCompleteRow(visibility_grid.front());
    //Mark right and left columns
    for_each(begin(visibility_grid), end(visibility_grid), [](auto &row){
        row.front() = row.back() = 1;
    });visibility_grid.front().assign(size(visibility_grid.front()), 1);
    //Mark bottom row
    MarkCompleteRow(visibility_grid.back()); 
}

void MarkVisibilityHorizontally(auto first, auto last, auto visibility_status_iter)
{
    auto max_height_so_far = *first;
    transform(
        first + 1, /*first1 : skip left/right border element */
        last - 1, /*last1  : skip right/left border element */
        visibility_status_iter + 1, /*first2 : current visisbility status
                                    might be possible it is already
                                    visible from some other direction*/
        visibility_status_iter + 1, /*output : position to store visibility status*/
        [&max_height_so_far](const auto &current_tree_height, const auto &kCurrentVisibilityStatus)
        {
            const auto kIsVisible   = max_height_so_far < current_tree_height;
            max_height_so_far       = max(max_height_so_far, current_tree_height);
            return kCurrentVisibilityStatus || kIsVisible;
        }
    );
}

void MarkVisibilityFromLeft(const vector<int> &row_from_grid, vector<bool> &visibility_row)
{
    MarkVisibilityHorizontally(cbegin(row_from_grid), cend(row_from_grid), begin(visibility_row));
}

void MarkVisibilityFromRight(const vector<int> &row_from_grid, vector<bool> &visibility_row)
{
    MarkVisibilityHorizontally(crbegin(row_from_grid), crend(row_from_grid), rbegin(visibility_row));
}

void MarkVisibilityVertically(const size_t &col_idx, auto first, auto last, auto visibility_grid_iter)
{
    auto max_height_so_far = (*first)[col_idx];
    ++first;
    ++visibility_grid_iter;
    while (first != last - 1)
    {
        const auto &kCurrentRow     = *first;
        auto &visibility_status_row = *visibility_grid_iter;
        visibility_status_row[col_idx] = visibility_status_row[col_idx] 
                                                || (max_height_so_far < kCurrentRow[col_idx]); 
        max_height_so_far = max(max_height_so_far, kCurrentRow[col_idx]);
        
        ++first;
        ++visibility_grid_iter;
    }
}

void MarkVisibilityFromTop(const int &col_idx, const vector<vector<int>> &grid,
                           vector<vector<bool>> &visibility_status_grid)
{
    MarkVisibilityVertically(col_idx, cbegin(grid), cend(grid), begin(visibility_status_grid));
}

void MarkVisibilityFromBottom(const int &col_idx, const vector<vector<int>> &grid,
                           vector<vector<bool>> &visibility_status_grid)
{
    MarkVisibilityVertically(col_idx, crbegin(grid), crend(grid), rbegin(visibility_status_grid));
}

size_t Count1s(const vector<vector<bool>> &grid)
{
    return accumulate(cbegin(grid), cend(grid), 0, [](const auto init, const auto &row){
            return init + accumulate(cbegin(row), cend(row), 0);
    });
}

size_t CountNumberOfVisibleTrees(const vector<vector<int>> &grid)
{
    auto visibility_grid = vector<vector<bool>>{ size(grid), vector<bool>(size(grid[0]), 0) };
    InitializeVisibilty(visibility_grid);
    Print(visibility_grid);
    const auto kTotalRows = size(grid);
    const auto kTotalColumns = size(grid[0]);
    for (auto idx = 1; idx < kTotalRows - 1; ++idx)
    {
        MarkVisibilityFromLeft(grid[idx], visibility_grid[idx]);
        MarkVisibilityFromRight(grid[idx], visibility_grid[idx]);
    }
    for (auto idx = 1; idx < kTotalColumns - 1; ++idx)
    {
        MarkVisibilityFromTop(idx, grid, visibility_grid);
        MarkVisibilityFromBottom(idx, grid, visibility_grid);
    }
    return Count1s(visibility_grid);
}

int main(int argc, const char *args[])
{
    auto grid = ReadGridFromFile(args[1]);
    Print(grid);
    const auto kNumberOfVisibleTrees = CountNumberOfVisibleTrees(grid);
    cout << "Part 1, no. of visible trees: " << kNumberOfVisibleTrees << endl;

    return 0;
}