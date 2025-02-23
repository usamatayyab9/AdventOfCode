#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string_view>
#include <sstream>
#include <tuple>
#include <numeric>

using std::all_of;
using std::accumulate;
using std::cout;
using std::cbegin;
using std::crbegin;
using std::crend;
using std::end;
using std::endl;
using std::ifstream;
using std::size;
using std::string;
using std::stringstream;
using std::string_view;
using std::vector;
using std::tuple;

struct XYPoint
{
    int x;
    int y;
};

struct RobotInfo
{
    XYPoint position;
    XYPoint velocity;
};

vector<string> SplitString(const string &str, const char &delimiter)
{
    auto tokens = vector<string>{};
    auto token  = string{};
    auto ss     = stringstream{ str };
    
    while (getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }

    return tokens;
}

RobotInfo ProcessString(const string &str)
{
    //p=61,54 v=-38,-87
    auto tokens = SplitString(str, ' '); // tokens = { "p=61,54", "v=-38,-87" }
    tokens[0].erase(0, 2);               // tokens = { "61,54"  , "v=-38,-87" }
    tokens[1].erase(0, 2);               // tokens = { "61,54"  , "-38,-87"   }
    auto position = SplitString(tokens[0], ','); // position = { "61", "54" }
    auto velocity = SplitString(tokens[1], ','); // velocity = { "-38", "-87" }
    return RobotInfo
    {
        { std::stoi(position[0]), std::stoi(position[1]) },
        { std::stoi(velocity[0]), std::stoi(velocity[1]) }
    };
}

vector<RobotInfo> ReadInput(string_view filename)
{
    auto fin    = ifstream{ filename.data() };
    auto robots = vector<RobotInfo>{};
     
    for (auto line = string{}; getline(fin, line);)
    {
        robots.push_back(ProcessString(line));
    }

    return robots;
}

void Print(const vector<vector<int>> &grid)
{
    for (const auto &row : grid)
    {
        for (const auto &cell : row)
        {
            if (cell == 0)
            {
                cout << '.';
            }
            else
            {
                cout << cell;
            }
            cout << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

int CalculateNewPosition(const int &kPosition, const int &kVelocity, const int &kMaxIterations, const int &kMaxValue)
{
    return (((kPosition + (kVelocity * kMaxIterations)) % kMaxValue) + kMaxValue) % kMaxValue;
}

template <class IteratorType>
size_t CalculateRobotCountInQuadrant(IteratorType first, IteratorType last, const bool &kInReverse)
{
    return accumulate(first, last, 0, [kInReverse](const auto &acc, const auto &row)
    {
        if (kInReverse)
        {
            return accumulate(crbegin(row), crbegin(row) + size(row) / 2, acc);
        }
        else
        {
            return accumulate(cbegin(row), cbegin(row) + size(row) / 2, acc);
        }
    });
}

tuple<size_t, size_t, size_t, size_t> CalculateRobotCountForEachQuadrant(const vector<vector<int>> &kGrid)
{
    const auto kMidRow  = size(kGrid) / 2;
    const auto q1_count = CalculateRobotCountInQuadrant(cbegin(kGrid), cbegin(kGrid) + kMidRow, false);
    const auto q2_count = CalculateRobotCountInQuadrant(cbegin(kGrid), cbegin(kGrid) + kMidRow, true);
    const auto q3_count = CalculateRobotCountInQuadrant(crbegin(kGrid), crbegin(kGrid) + kMidRow, false);
    const auto q4_count = CalculateRobotCountInQuadrant(crbegin(kGrid), crbegin(kGrid) + kMidRow, true);

    return { q1_count, q2_count, q3_count, q4_count };
}

void Execute_N_Iterations(vector<RobotInfo> &kRobotsInfo, vector<vector<int>> &grid, const int &kIterations)
{
    for (auto &[position, velocity] : kRobotsInfo)
    {
        const auto kNewX = CalculateNewPosition(position.x, velocity.x, kIterations, size(grid[0]));
        const auto kNewY = CalculateNewPosition(position.y, velocity.y, kIterations, size(grid));
        grid.at(kNewY).at(kNewX)           += 1;
        grid.at(position.y).at(position.x) -= 1;

        position = { kNewX, kNewY };
    }
}

vector<vector<int>> CreateGrid(const int &kRowCount, const int &kColumnCount, const vector<RobotInfo> &kRobotsInfo)
{
    auto grid = vector<vector<int>>(kRowCount, vector<int>(kColumnCount, 0));
    for (const auto &[kPosition, kVelocity] : kRobotsInfo)
    {        
        grid.at(kPosition.y).at(kPosition.x) += 1;
    }
    return grid;
}

size_t Day14Part1(const vector<RobotInfo> &kRobotsInfo, const int &kRowCount, const int &kColumnCount)
{
    constexpr int kMaxIterations = 100;
    auto grid                    = CreateGrid(kRowCount, kColumnCount, kRobotsInfo);
    auto robots                  = kRobotsInfo;

    Execute_N_Iterations(robots, grid, kMaxIterations);
    
    const auto [kQ1Count, kQ2Count, kQ3Count, kQ4Count] = CalculateRobotCountForEachQuadrant(grid);
    return kQ1Count * kQ2Count * kQ3Count * kQ4Count;
}

size_t Day14Part2(const vector<RobotInfo> &kRobotsInfo, const int &kRowCount, const int &kColumnCount)
{
    auto grid   = CreateGrid(kRowCount, kColumnCount, kRobotsInfo);
    auto robots = kRobotsInfo;
    auto iteration = 0;
    for (auto ii = 0; ii < 10000 ;++ii)
    {
        Execute_N_Iterations(robots, grid, 1);
        if (all_of(cbegin(grid), cend(grid), [](const auto &row){ return all_of(cbegin(row), cend(row), [](const auto &cell){ return cell <= 1; }); }))
        {
            Print(grid);
            iteration = ii + 1;
            break;
        }

    }
    return iteration;
}

int main(int argc, const char *args[])
{
    const auto kRobots = ReadInput(args[1]);
    cout << "Day 14, Part 1 : " << Day14Part1(kRobots, 103, 101) << endl;
    cout << "Day 14, Part 2 : " << Day14Part2(kRobots, 103, 101);
    return 0;
}