/**
 * @file day_10_hoof_it.cpp
 * @brief This program reads a character matrix from a file specified as a command line argument
 * and prints the matrix to the console. The matrix is read from the file line by line,
 * where each line represents a row in the matrix.
 *  * 
 * Compile command : g++ -std=c++17 .\day_10_hoof_it.cpp
 * 
 * @note The file should contain a matrix of characters, with each line representing a row.
 * 
 * @author Usama Tayyab
 * @date 2024
 */
// This program reads a character matrix from a file specified as a command line argument
// and prints the matrix to the console. The matrix is read from the file line by line,
// where each line represents a row in the matrix.

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <array>
#include <set>
#include <functional>

using std::array;
using std::begin;
using std::cerr;
using std::cout;
using std::end;
using std::endl;
using std::ifstream;
using std::pair;
using std::queue;
using std::set;
using std::size;
using std::string;
using std::vector;

/**
 * @brief Read a character matrix from a file.
 *
 * @param filename The name of the file to read from.
 * @return A 2D vector of characters representing the matrix read from the file.
 */
auto ReadMatrixFromFile(const string& filename) -> vector<vector<char>>
{
    auto fin    = ifstream{ filename };
    auto matrix = vector<vector<char>>{};
    for (auto line = string{}; getline(fin, line);)
    {
        matrix.push_back({ begin(line), end(line) });
    }
    return matrix;
}

template <typename T>
bool IsValueInRange(T value, T lower_bound, T upper_bound)
{
    return value >= lower_bound && value < upper_bound;
}

template <class TargetNodeProcessor>
void TraversePaths(const vector<vector<char>>& matrix, const int &ii, const int &jj, TargetNodeProcessor target_processor)
{
    constexpr auto kDirections  = array<pair<int, int>, 4>{ { { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 } } };
    auto indices_queue          = queue<pair<int, int>>{};
    indices_queue.push({ ii, jj });
    while (false == indices_queue.empty())
    {
        const auto [i, j] = indices_queue.front();
        indices_queue.pop();
        if (const auto kValue = matrix[i][j]; '9' == kValue) { target_processor(i, j); }
        else
        {
            for (const auto& [di, dj] : kDirections)
            {
                const auto new_i = i + di;
                const auto new_j = j + dj;
                if (IsValueInRange(new_i, 0, static_cast<int>(size(matrix))) &&
                    IsValueInRange(new_j, 0, static_cast<int>(size(matrix))) &&
                    matrix[new_i][new_j] == kValue + 1
                    )
                {
                    indices_queue.push({ new_i, new_j });
                }
            }
        }
    }
}

size_t CountPathsPart1(const vector<vector<char>>& matrix, const int &ii, const int &jj)
{
    auto indices_set = set<pair<int, int>>{};
    TraversePaths(matrix, ii, jj, [&indices_set](const auto &row_idx, const auto &col_idx){
        indices_set.insert({ row_idx, col_idx });
    });
    return size(indices_set);
}


size_t CountPathsPart2(const vector<vector<char>>& matrix, const int &ii, const int &jj)
{
    auto path_count = size_t{ 0 };
    TraversePaths(matrix, ii, jj, [&path_count](const auto &row_idx, const auto &col_idx){
        ++path_count;
    });
    return path_count;
}

template<class PartFunction>
size_t Day10Solution(const vector<vector<char>> &matrix, PartFunction path_count_function)
{
    auto score_sum  = size_t{ 0 };
    for (auto ii = 0; ii < size(matrix); ++ii)
    {
        for (auto jj = 0; jj < size(matrix[0]); ++jj)
        {
            if ('0' == matrix[ii][jj])
            {
                score_sum += path_count_function(matrix, ii, jj);
            }
        }
    }
    return score_sum;
}

size_t Day10Part1(const vector<vector<char>> &matrix)
{
    return Day10Solution(matrix, CountPathsPart1);
}

size_t Day10Part2(const vector<vector<char>> &matrix)
{
    return Day10Solution(matrix, CountPathsPart2);
}

int main(int argc, char* argv[])
{
    const auto kMatrix = ReadMatrixFromFile(argv[1]);
    cout << "Day 10, Part 1 : " << Day10Part1(kMatrix) << endl;
    cout << "Day 10, Part 2 : " << Day10Part2(kMatrix) << endl;
    return 0;
}