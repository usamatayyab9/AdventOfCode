/**
 * @file day_15_warehouse_woes.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief Solution for Advent of Code - Day 15 "Warehouse Woes"
 *
 * GENERAL STRATEGY:
 * This program simulates the movement of a robot in a warehouse grid based on a set of movement instructions.
 * The warehouse is represented as a 2D grid where:
 *   - '#' represents walls (obstacles that cannot be crossed).
 *   - '@' represents the robot's starting position.
 *   - '.' represents empty spaces where the robot can move.
 *   - 'O' is used in Part 1 to track robot movement.
 *   - '[ ]' is used in Part 2 to handle expanded grids.
 *
 * The solution consists of two parts:
 * 1. Part 1: Moves the robot following simple rules and tracks visited locations.
 * 2. Part 2: Expands the grid and implements a more complex movement strategy.
 *
 * The program reads input from a file containing:
 *   - The warehouse grid.
 *   - A sequence of movement instructions ('^', 'v', '<', '>').
 *
 * It calculates and prints a final value based on robot movement.
 *
 * Compile: g++ -std=c++17 day_15_warehouse_woes.cpp -o day_15_warehouse_woes
 * Run: ./day_15_warehouse_woes <filename>.txt
 */
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <string_view>
#include <utility>
#include <vector>
#include <iterator>
#include <set>
#include <type_traits>

using std::all_of;
using std::begin;
using std::conditional_t;
using std::copy;
using std::cout;
using std::end;
using std::endl;
using std::find;
using std::getline;
using std::ifstream;
using std::istream_iterator;
using std::pair;
using std::set;
using std::string;
using std::string_view;
using std::vector;

struct MatrixIndex
{
    size_t row;
    size_t col;
    bool operator<(const MatrixIndex &rhs) const { return row < rhs.row || (row == rhs.row && col < rhs.col); }
    bool operator==(const MatrixIndex &rhs) const { return row == rhs.row && col == rhs.col; }
};

pair<vector<vector<char>>, vector<char>> ReadGrid(ifstream &fin)
{
    auto grid = vector<vector<char>>{};
    for (auto line = string{}; getline(fin, line) && size(line) > 0;)
    {
        grid.push_back(vector<char>{begin(line), end(line)});
    }
    return { grid, { istream_iterator<char>{ fin }, istream_iterator<char>{} } };
}

MatrixIndex FindRobotPosition(const vector<vector<char>> &grid)
{
    auto robot = MatrixIndex{ size(grid), size(grid) };
    for (auto row = size_t{ 0 }; row < size(grid); ++row)
    {
        if (auto col_pos = find(cbegin(grid[row]), cend(grid[row]), '@'); col_pos != cend(grid[row]))
        {
            robot = { row, static_cast<size_t>(col_pos - cbegin(grid[row])) };
            break;
        }
    }
    return robot;
}

bool IsValueInRange(const size_t &value, const size_t &min, const size_t &max)
{
    return value >= min && value < max;
}

bool IsValidMatrixIndex(const MatrixIndex &index, const vector<vector<char>> &grid)
{
    return IsValueInRange(index.row, 0, size(grid)) && IsValueInRange(index.col, 0, size(grid[index.row]));
}

namespace Part1
{
    /**
     * @brief Generic function solving part 1. Moves the robot in the grid based on the given row and column functions.
     * For each movement, the robot moves in the given direction until it hits a wall or an empty space('.').
     * When an empty space is found, the robot set that position to 'O' and moves to the new position and sets it to '@'.
     * Then, it sets the previous position to '.'.
     * 
     * Consider the following example:
     * grid = 
     * ################
     * ##...OOOOO@...##
     * ################
     * 
     * Lets say we want to move the robot to left.
     * 
     * The function will be called with the following arguments:
     * MoveRobot(grid, { 1, 10 }, [](auto row) { return row; }, [](auto col) { return col - 1; });
     * 
     * Walkthrough:
     * 1. new_position = { 1, 9 }
     * 2. first_dot_pos = { 1, 4 }
     * 3. Once an empty space is found, three positions are updated inside if:
     *      3.1 Set first_dot_pos to 'O'. grid[1][4] = 'O'
     *            ################
     *            ##..OOOOOO@...##
     *            ################
     *      3.2 Set new_position to '@'. grid[1][9] = '@'
     *           ################
     *           ##..OOOOO@@...##
     *           ################
     *      3.3 Set current_robot_position to '.'. grid[1][10] = '.'
     *          ################
     *          ##..OOOOO@....##
     *          ################
     * 4. current_robot_position = { 1, 9 }
     * 
     * Similar execution follows when moving right, up and down.
     *          
     * 
     * @tparam RowFunction 
     * @tparam ColumnFunction 
     * @param grid 
     * @param current_robot_position 
     * @param row_function 
     * @param column_function 
     */
template <class RowFunction, class ColumnFunction>
void MoveRobot(vector<vector<char>> &grid, MatrixIndex &current_robot_position, RowFunction row_function, ColumnFunction column_function)
{
    auto new_position = MatrixIndex{ row_function(current_robot_position.row), column_function(current_robot_position.col) };
    for (auto first_dot_pos = new_position;
        IsValidMatrixIndex(first_dot_pos, grid);
        first_dot_pos = { row_function(first_dot_pos.row), column_function(first_dot_pos.col) })
    {
        if ('#' == grid[first_dot_pos.row][first_dot_pos.col])
        {
            break;
        }
        if ('.' == grid[first_dot_pos.row][first_dot_pos.col])
        {
            grid[first_dot_pos.row][first_dot_pos.col]                   = 'O';
            grid[new_position.row][new_position.col]                     = '@';
            grid[current_robot_position.row][current_robot_position.col] = '.';
            current_robot_position = new_position;
            break;
        }
    }
}

/**
 * @brief Helper function for moving the robot up.
 * 
 * @param grid 
 * @param current_robot_position 
 */
void MoveRobotUp(vector<vector<char>> &grid, MatrixIndex &current_robot_position)
{
    MoveRobot(grid, current_robot_position, [](auto row) { return row - 1; }, [](auto col) { return col; });
}

/**
 * @brief Helper function for moving the robot down.
 * 
 * @param grid 
 * @param current_robot_position 
 */
void MoveRobotDown(vector<vector<char>> &grid, MatrixIndex &current_robot_position)
{
    MoveRobot(grid, current_robot_position, [](auto row) { return row + 1; }, [](auto col) { return col; });
}

/**
 * @brief Helper function for moving the robot left.
 * 
 * @param grid 
 * @param current_robot_position 
 */
void MoveRobotLeft(vector<vector<char>> &grid, MatrixIndex &current_robot_position)
{
    MoveRobot(grid, current_robot_position, [](auto row) { return row; }, [](auto col) { return col - 1; });
}

/**
 * @brief Helper function for moving the robot right
 * 
 * @param grid 
 * @param current_robot_position 
 */
void MoveRobotRight(vector<vector<char>> &grid, MatrixIndex &current_robot_position)
{
    MoveRobot(grid, current_robot_position, [](auto row) { return row; }, [](auto col) { return col + 1; });
}

};

size_t CalculateSumOfGrid(const vector<vector<char>> &grid, const char &kBlockCharacter)
{
    auto sum = size_t{ 0 };
    for (auto row = size_t{ 0 }; row < size(grid); ++row)
    {
        for (auto col = size_t{ 0 }; col < size(grid[row]); ++col)
        {
            if (kBlockCharacter == grid[row][col])
            {
                sum += (row * 100) + col;
            }
        }
    }
    return sum;
}

namespace Part2
{
vector<vector<char>> ScaleGrid(const vector<vector<char>> &grid)
{
    auto scaled_grid = vector<vector<char>>{};
    for (auto row = size_t{ 0 }; row < size(grid); ++row)
    {
        auto new_row = vector<char>{};
        for (auto col = size_t{ 0 }; col < size(grid[row]); ++col)
        {
            if      ('#' == grid[row][col]) { new_row.push_back('#'); new_row.push_back('#'); }
            else if ('O' == grid[row][col]) { new_row.push_back('['); new_row.push_back(']'); }
            else if ('.' == grid[row][col]) { new_row.push_back('.'); new_row.push_back('.'); }
            else if ('@' == grid[row][col]) { new_row.push_back('@'); new_row.push_back('.'); }
        }
        scaled_grid.push_back(new_row);
    }
    return scaled_grid;
}

pair<size_t, size_t> GetBracketsPosition(const vector<vector<char>> &kGrid, const size_t &kRowIdx, const size_t &kColIdx)
{
    auto left_bracket_idx  = size_t{ 0 };
    auto right_bracket_idx = size_t{ 0 };

    if ('[' == kGrid[kRowIdx][kColIdx]) { left_bracket_idx = kColIdx    ; right_bracket_idx = kColIdx + 1; }
    else                                { left_bracket_idx = kColIdx - 1; right_bracket_idx = kColIdx;     }

    return { left_bracket_idx, right_bracket_idx };
}


enum class VerticalDirection { kUp = -1, kDown = 1 };

template<VerticalDirection T> using Comparator = std::conditional_t<T == VerticalDirection::kUp, std::less<>, std::greater<>>;
template<VerticalDirection T> using VerticalIndicesSet = set<pair<MatrixIndex, MatrixIndex>, Comparator<T>>;

template<VerticalDirection kDirection>
bool CanMoveVertically(const vector<vector<char>> &kGrid, const MatrixIndex &kCurrentPosition, VerticalIndicesSet<kDirection> &index_porjections)
{
    const auto kRowIdx = kCurrentPosition.row;
    const auto kColIdx = kCurrentPosition.col;
    auto can_move      = false;

    if      ('.' == kGrid[kRowIdx][kColIdx]) { can_move = true; }
    else if ('#' == kGrid[kRowIdx][kColIdx]) { can_move = false; }
    else
    {
        const auto [kLeftBracketIdx, kRightBracketIdx] = GetBracketsPosition(kGrid, kRowIdx, kColIdx);
        constexpr auto kRowOffset                      = static_cast<size_t>(kDirection);

        can_move = CanMoveVertically<kDirection>(kGrid, { kRowIdx + kRowOffset, kLeftBracketIdx }, index_porjections)
            && CanMoveVertically<kDirection>(kGrid, { kRowIdx + kRowOffset, kRightBracketIdx }, index_porjections)
        ;
        
        if (can_move)
        {
            const auto kProjection1 = pair{ MatrixIndex{ kRowIdx, kLeftBracketIdx }, MatrixIndex{ kRowIdx + kRowOffset, kLeftBracketIdx } };
            const auto kProjection2 = pair{ MatrixIndex{ kRowIdx, kRightBracketIdx }, MatrixIndex{ kRowIdx + kRowOffset, kRightBracketIdx } };
            index_porjections.insert({kProjection1, kProjection2});
        }
        else
        {
            index_porjections.clear();
        }
    }
    return can_move;
}

template <VerticalDirection kDirection>
void MoveRobotVertically(vector<vector<char>> &grid, MatrixIndex &current_position)
{
    constexpr auto kRowOffset = static_cast<int>(kDirection);
    if (auto projections = VerticalIndicesSet<kDirection>{};
        CanMoveVertically<kDirection>(grid, MatrixIndex{ current_position.row + kRowOffset, current_position.col }, projections))
    {
        for (const auto &[kMatrixIdx1, kMatrixIdx2] : projections)
        {
            std::swap(grid[kMatrixIdx1.row][kMatrixIdx1.col], grid[kMatrixIdx2.row][kMatrixIdx2.col]);
        }
        grid[current_position.row + kRowOffset][current_position.col] = '@';
        grid[current_position.row][current_position.col]              = '.';
        current_position.row                                         += kRowOffset;
    }
}

enum class HorizontalDirection { kLeft = -1, kRight = 1 };

template <HorizontalDirection kDirection>
void MoveRobotHorizontally(vector<vector<char>> &grid, MatrixIndex &current_Position)
{
    constexpr auto direction = static_cast<int>(kDirection);
    auto &row                = grid[current_Position.row];
    for (auto idx = current_Position.col; IsValueInRange(idx, 0, size(row)) &&  '#' != row[idx]; idx += direction)
    {
        if ('.' == row[idx])
        {
            for (auto ii = idx; ii != current_Position.col; ii -= direction)
            {
                row[ii] = row[ii - direction];
            }
            row[current_Position.col + direction] = '@';
            row[current_Position.col]     = '.';
            current_Position.col         += direction;
            break;
        }
    }
}
};

enum class Solution { kPart1, kPart2 };
template<Solution kSolution>
size_t Day15Solve(vector<vector<char>> &grid, const vector<char> &movements)
{
    using Part2::VerticalDirection;
    using Part2::HorizontalDirection;
    constexpr auto UpFunction    = kSolution == Solution::kPart1 ? Part1::MoveRobotUp    : Part2::MoveRobotVertically<VerticalDirection::kUp>;
    constexpr auto DownFunction  = kSolution == Solution::kPart1 ? Part1::MoveRobotDown  : Part2::MoveRobotVertically<VerticalDirection::kDown>;
    constexpr auto LeftFunction  = kSolution == Solution::kPart1 ? Part1::MoveRobotLeft  : Part2::MoveRobotHorizontally<HorizontalDirection::kLeft>;
    constexpr auto RightFunction = kSolution == Solution::kPart1 ? Part1::MoveRobotRight : Part2::MoveRobotHorizontally<HorizontalDirection::kRight>;
    
    auto robot = FindRobotPosition(grid);
    for (const auto robot_movement : movements)
    {
        if      ('^' == robot_movement) { UpFunction(grid, robot);    }
        else if ('v' == robot_movement) { DownFunction(grid, robot);  }
        else if ('<' == robot_movement) { LeftFunction(grid, robot);  }
        else if ('>' == robot_movement) { RightFunction(grid, robot); }
    }

    return CalculateSumOfGrid(grid, kSolution == Solution::kPart1 ? 'O' : '[');
}

size_t Day15Part1(vector<vector<char>> grid, const vector<char> &kMovements)
{
    return Day15Solve<Solution::kPart1>(grid, kMovements);
}

size_t Day15Part2(const vector<vector<char>> &grid, const vector<char> &kMovements)
{
    auto new_grid   = Part2::ScaleGrid(grid);
    return Day15Solve<Solution::kPart2>(new_grid, kMovements);
}

int main(int argc, const char *args[])
{
    if (auto fin = ifstream{ args[1] }; fin)
    {
        auto [grid, movements] = ReadGrid(fin);
        cout << "Day 15, Part 1: " << Day15Part1(grid, movements) << endl;
        cout << "Day 15, Part 2: " << Day15Part2(grid, movements) << endl;
    }

    return 0;
}
