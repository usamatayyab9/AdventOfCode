/**
 * @file day_4_ceres_search.cpp
 * @author usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compile command : g++ -std=c++17 day_4_ceres_search.cpp
 * @date 2024-12-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <string_view>
#include <string>
#include <iterator>
#include <utility>
#include <array>

using std::array;
using std::back_inserter;
using std::begin;
using std::cbegin;
using std::cout;
using std::cend;
using std::end;
using std::endl;
using std::ifstream;
using std::istream_iterator;
using std::pair;
using std::size;
using std::string;
using std::string_view;
using std::transform;
using std::vector;

bool IsValueInRange(int value, int min, int max)
{
    return value >= min && value < max;
}

vector<vector<char>> ReadInput(string_view filename)
{
    auto fin   = ifstream{ filename.data() };
    auto input = vector<vector<char>>{};
    transform(istream_iterator<string>{ fin }, istream_iterator<string>{},
        back_inserter(input),
        [](auto line) {
            return vector<char>{ cbegin(line), cend(line) };
        }
    );
    
    return input;
}

bool ContainsInUniDirection(const vector<vector<char>>& input, const int &ii, const int &jj, string_view to_find, const int &offset_x, const int &offset_y)
{
    auto row_idx    = ii;
    auto col_idx    = jj;
    auto target_idx = 0;
    while (IsValueInRange(row_idx, 0, size(input)) && IsValueInRange(col_idx, 0, size(input[ii]))
        && input[row_idx][col_idx] == to_find[target_idx])
    {
        row_idx    += offset_x;
        col_idx    += offset_y;
        target_idx += 1;
    }
    return target_idx == size(to_find);
}

/**
 * @brief Counts the occurrences of a given string in a 2D grid of characters, 
 *        searching in all 8 possible directions from each character.
 * 
 * @param input A 2D vector of characters representing the grid to search within.
 * @param to_find A string_view representing the string to search for in the grid.
 * @return int The total count of occurrences of the string in the grid.
 */
int Day4Part1(const vector<vector<char>>& input, string_view to_find)
{
    constexpr auto kOffsetsArray = array<pair<int, int>, 8>
    {{
        { -1, -1 }, { -1, 0 }, { -1, +1 }, 
        { 0 , -1 },            { 0 , +1 } ,
        { +1, -1 }, { +1, 0 }, { +1, +1 } 
    }};

    auto count = 0;
    for (auto ii = 0; ii < size(input); ++ii)
    {
        for (auto jj = 0; jj < size(input[ii]); ++jj)
        {
            if (input[ii][jj] == to_find[0])
            {
                for (const auto& [dx, dy] : kOffsetsArray)
                {
                    count += ContainsInUniDirection(input, ii, jj, to_find, dx, dy);
                }
            }
        }
    }

    return count;
}


/**
 * @brief Checks if the diagonal elements around a given position in a 2D vector match specific patterns.
 *
 * This function checks if the elements diagonally adjacent to the position (ii, jj) in the input 2D vector
 * match one of the following patterns:
 * - 'S' at top-left and 'M' at bottom-right OR 'M' at top-left and 'S' at bottom-right
 * AND
 * - 'S' at top-right and 'M' at bottom-left OR 'M' at top-right and 'S' at bottom-left
 *
 * @param input A 2D vector of characters to be checked.
 * @param ii The row index of the central position.
 * @param jj The column index of the central position.
 * @return true if the diagonal elements match one of the specified patterns, false otherwise.
 */
bool AreDiagonalElementsMatching(const vector<vector<char>>& input, const int &ii, const int &jj)
{
    return (('S' == input[ii - 1][jj - 1] &&  'M' == input[ii + 1][jj + 1]) || ('M' == input[ii - 1][jj - 1] &&  'S' == input[ii + 1][jj + 1])) &&
           (('S' == input[ii - 1][jj + 1] &&  'M' == input[ii + 1][jj - 1]) || ('M' == input[ii - 1][jj + 1] &&  'S' == input[ii + 1][jj - 1]));
}

bool AreNeighbouringIndicesValid(const vector<vector<char>>& input, const int &ii, const int &jj)
{
    return IsValueInRange(ii - 1, 0, size(input)) && IsValueInRange(jj - 1, 0, size(input[ii])) &&
           IsValueInRange(ii + 1, 0, size(input)) && IsValueInRange(jj + 1, 0, size(input[ii]));
}

int Day4Part2(const vector<vector<char>>& input)
{
    auto count = 0;
    for (auto ii = 0; ii < size(input); ++ii)
    {
        for (auto jj = 0; jj < size(input[ii]); ++jj)
        {
            if ('A' == input[ii][jj] && AreNeighbouringIndicesValid(input, ii, jj) && AreDiagonalElementsMatching(input, ii, jj))
            {
                count += 1;
            }
        }
    }
    return count;
}

int main(int argc, const char* args[])
{
    const auto kInputMatrix = ReadInput(args[1]);
    cout << "Day 4, Part 1: " << Day4Part1(kInputMatrix, "XMAS") << endl;
    cout << "Day 4, Part 2: " << Day4Part2(kInputMatrix) << endl;
    return 0;
}