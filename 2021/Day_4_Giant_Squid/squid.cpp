/**
 * @file squid.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * This code is the solution of @link https://adventofcode.com/2021/day/4 @endlink
 * The following is compiled on g++ version 9.4.0. 
 * Compilation command : g++ -std=c++17 squid.cpp
 * The program takes input the name of file as command line argument.
 * For example if a.out is the binary obtained by compiling this program,
 * then it should be run as:
 *                  a.out test.txt
 *                  a.out input.txt
 * @date 2022-01-11
 * @copyright Copyright (c) 2022
 */ 
#include <fstream>
#include <iostream>
#include <iterator>
#include <charconv>
#include <algorithm>
#include <vector>
#include <sstream>
#include <functional>
#include <numeric>
using namespace std;

vector<int> ParseCSV(string str)
{
    std::replace(begin(str), end(str), ',', ' ');
    stringstream ss(str);
    return {istream_iterator<int>{ss}, istream_iterator<int>{}};
}

using Grid = vector<vector<int>>;

void Read_N_SizeGrid(istream &in, Grid &grid, int N = 5)
{
    grid.clear();
    grid.assign(N, vector<int>(N, 0));
    for (auto &vec : grid)
    {
        copy_n(istream_iterator<int>{in}, N, begin(vec));
    }
}

void print(const Grid &g)
{
    for (const auto &vec : g)
    {
        copy(cbegin(vec), cend(vec), ostream_iterator<remove_reference_t<decltype(vec)>::value_type>{cout, " "});
        cout << endl;
    }
}

struct BingoResult
{
    long idx;
    int last_num_called;
};

void MarkGrid(Grid &g, const int num)
{
    for (auto &vec : g)
    {
        replace(begin(vec), end(vec), num, -1);
    }
}

bool HasGridWon(const Grid &g)
{
    auto rows_result = any_of(cbegin(g), cend(g), [](const auto &vec){ 
        return all_of(cbegin(vec), cend(vec), bind(equal_to{}, -1, std::placeholders::_1));
    });
    auto cols_result = false;
    if (rows_result == false)
    {
        for (size_t i{0}; i < g.size() ;++i)
        {
            auto j = size_t{0};
            for (j = 0; j < g.size() && g[j][i] == -1 ; ++j) { continue; }
            if  (j == g.size())
            {
                cols_result = true;
                break;
            }
        }
    }
    return rows_result || cols_result;
}

BingoResult PlayBingo(vector<Grid> &grids, const vector<int> &list)
{
    auto results = BingoResult{grids.size(), -1};
    for (const auto &elem : list)
    {
        for_each(begin(grids), end(grids), [&elem] (Grid &g) { MarkGrid(g, elem); });
        if (auto pos = find_if(cbegin(grids), cend(grids), HasGridWon); pos != cend(grids))
        {
            results = {pos - cbegin(grids), elem};
            break;
        }
    }
    return results;
}

int LastBoardtoWin(vector<Grid> &grids, const vector<int> &list)
{
    auto results = -1;
    auto is_last_grid_remaining = false;
    for (const auto &elem : list)
    {
        is_last_grid_remaining = (grids.size() == 1);
        for_each(begin(grids), end(grids), [&elem] (Grid &g) { MarkGrid(g, elem); });
        if (is_last_grid_remaining == false)
        {
            auto pos = remove_if(begin(grids), end(grids), HasGridWon);
            grids.erase(pos, grids.cend());
        }
        else
        {
            if (HasGridWon(grids[0]))
            {
                results = elem;
                break;
            }
        }
    }
    return results;
}

int main()
{
    auto fin = ifstream{"input.txt"};
    auto str = string{""};
    getline(fin, str, '\n');
    cout << "Parsing comma string to vector \n";
    vector<int> vec{ParseCSV(str)};
    str.clear();
    cout << "reading grids...\n";
    auto grids = vector<Grid>{};
    while (fin)
    {
        Grid g;
        Read_N_SizeGrid(fin, g, 5);
        grids.emplace_back(std::move(g));
    }
    grids.pop_back();
    cout << "Total grids read: " << grids.size() << "\n";
    // for_each(grids.cbegin(), grids.cend(), print);
    // BingoResult r{PlayBingo(grids, vec)};
    auto last_number_taken = LastBoardtoWin(grids, vec);
    auto &winning_grid = grids[0];
    print(winning_grid);
    auto sum_of_unamrked = accumulate(winning_grid.cbegin(), winning_grid.cend(), 0, 
        [](long init, const vector<int> &v)
        {
            return init + accumulate(v.cbegin(), v.cend(), 0, [](long init, int i){
                if (i == -1) { return init; }
                return init + i;
            });
        }
    );
    cout << "last number: " << last_number_taken << endl;
    cout << sum_of_unamrked * last_number_taken;
    return 0;
}