/**
 * @file venture.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * This code is the solution of @link https://adventofcode.com/2021/day/4 @endlink
 * The following is compiled on g++ version 9.4.0. 
 * Compilation command : g++ -std=c++17 venture.cpp
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

using namespace std;

class IntegeresExtractor
{
    public:
    IntegeresExtractor(const string &str): buffer{str}
    { }

    bool ExtractIntger(int &i)
    {
        if (idx < buffer.size())
        {
            auto first_digit = find_if(cbegin(buffer) + idx, cend(buffer), ::isdigit);
            auto last_digit = find_if_not(first_digit, cend(buffer), ::isdigit);
            auto start_idx = first_digit - cbegin(buffer);
            auto end_idx = last_digit - cbegin(buffer);
            idx = end_idx;
            from_chars(&buffer[start_idx], &buffer[end_idx], i);
            return true;
        }
        return false;
    }
    size_t idx{0};
    string buffer;
};

struct Point
{
    int x;
    int y;
};

struct Line
{
    Point pt1;
    Point pt2;
    bool IsHorizontal() const
    {
        return pt1.y == pt2.y;
    }

    bool IsVertical() const
    {
        return pt1.x == pt2.x;
    }

    bool IsDiagonal() const
    {
        return (abs(pt1.x - pt2.x) / abs(pt1.y - pt2.y)) == 1;
    }

};

string ReadAll()
{
    auto fin = ifstream{"input.txt"};
    return {istreambuf_iterator<char>{fin}, istreambuf_iterator<char>{}};
}

vector<int> ReadIntegers()
{
    auto i = -1;
    auto int_extractor = IntegeresExtractor{ReadAll()};
    auto raw = vector<int>{};
    while (int_extractor.ExtractIntger(i)) { raw.push_back(i); }
    return raw;
}

using Grid = vector<vector<int>>;

void drawLineOnGrid(Grid &grid, const Line &line)
{
    if (line.IsVertical())
    {
        auto row = line.pt1.x;
        auto [s, e] = std::minmax(line.pt1.y, line.pt2.y);
        for (auto col = s; col <= e; ++col)
        {
            ++grid[row][col];
        }
    }
    else if (line.IsHorizontal())
    {
        auto col = line.pt1.y;
        auto [s, e] = std::minmax(line.pt1.x, line.pt2.x);
        for (auto row = s; row <= e; ++row)
        {
            ++grid[row][col];
        }
    }
    else if (line.IsDiagonal())
    {
        auto inc_x = 0;
        auto row = line.pt1.x;
        if (line.pt1.x > line.pt2.x) { inc_x = -1; }
        else { inc_x = +1; }

        auto inc_y = 0;
        auto col = line.pt1.y;
        if (line.pt1.y < line.pt2.y) { inc_y = +1; }
        else { inc_y = -1; }
        
        for (; row != (line.pt2.x + inc_x) && col != (line.pt2.y + inc_y) ;row += inc_x, col += inc_y)
        {
            ++grid[row][col];
        }
    }
}

int main()
{
    
    // cout << data << endl;
    auto raw = ReadIntegers();
    auto points = vector<Point>{};
    points.reserve(raw.size() / 2);   
    for (auto idx = size_t{0}; idx < raw.size(); idx+=2)
    {
        points.push_back({raw[idx], raw[idx + 1]});
    }
    auto Lines = vector<Line>{};
    Lines.reserve(points.size() / 2);
    for (auto idx = size_t{0}; idx < points.size(); idx+=2)
    {
        Lines.push_back({points[idx], points[idx + 1]});
    }
    auto iter = remove_if(begin(Lines), end(Lines), [](const auto &line)
    {
        return !(line.IsVertical() || line.IsHorizontal() || line.IsDiagonal());
    });
    Lines.erase(iter, cend(Lines));
    auto max = *max_element(cbegin(raw), cend(raw));

    auto grid = vector<vector<int>>(max + 1, vector<int>(max + 1, 0));
    for (const auto &line : Lines)
    {
        drawLineOnGrid(grid, line);
    }

    auto total_count = 0;
    for (const auto &vec : grid)
    {
        total_count += count_if(cbegin(vec), cend(vec), [](const auto &elem){ return elem >= 2; });
    }
    cout << "Part 2 : " << total_count;
    return 0;
}