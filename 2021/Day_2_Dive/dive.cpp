/**
 * @file sonar_sweep.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * This code is the solution of @link https://adventofcode.com/2021/day/1 @endlink
 * The following is compiled on g++ version 9.4.0. 
 * Compilation command : g++ -std=c++17 dive.cpp
 * The program takes input the name of file as command line argument.
 * For example if a.out is the binary obtained by compiling this program,
 * then it should be run as:
 *                  a.out test.txt
 *                  a.out input.txt
 * @date 2022-01-11
 * @copyright Copyright (c) 2022
 */
#include <iostream>
#include <fstream>
#include <algorithm>
#include <utility>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

struct Command
{
    string instruction;
    int units;
};

struct coordinate
{
    int x;
    int y;
    int aim;
    void forward(int X)
    {
        x += X;
        y += aim * X;
    }
    void down(int X)
    {
        aim += X;
    }
    void up(int X)
    {
        aim -= X;
    }
};

vector<Command> ReadInput(string_view filename)
{
    auto commands = vector<Command>{};
    for (auto [fin, command, units] = tuple{ifstream{ filename.data() }, string{}, 0};
        fin >> command >> units;)
    {
        commands.push_back({ command, units });
    }
    return commands;
}

auto Part2(const vector<Command> &commands)
{
    auto position = coordinate{0, 0, 0};
    for (const auto &[command, units] : commands)
    {
        if      (command == "forward") { position.forward(units); }
        else if (command == "down"   ) { position.down(units);    }
        else if (command == "up"     ) { position.up(units);      }
    }
    return position.x * position.y;
}

int main(int argc, const char *args[])
{
    const auto kCommandsList = ReadInput(args[1]);
    cout << "Part 2 : " << Part2(kCommandsList) << endl;
    return 0;
}