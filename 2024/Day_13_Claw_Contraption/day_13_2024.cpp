/**
 * @file day_13_2024.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compile command : g++ -std=c++17 day_13_2024.cpp -o
 * Run command     : ./day_13_2024 input.txt 
 * @date 2025-02-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <string_view>
#include <vector>
#include <tuple>
#include <cmath>
#include <unordered_map>

using std::begin;
using std::cout;
using std::end;
using std::endl;
using std::getline;
using std::ifstream;
using std::pair;
using std::string;
using std::string_view;
using std::tuple;
using std::vector;

struct XYPoint
{
    long long int x;
    long long int y;

    bool operator==(const XYPoint &other) const
    {
        return x == other.x && y == other.y; 
    }
};

struct MachineConfiguration
{
    XYPoint button_A;
    XYPoint button_B;
    XYPoint target;
};

XYPoint ParseLine(const string &line)
{
    constexpr auto IsDigit = [](unsigned char c){ return std::isdigit(c); };
    auto first1 = std::find_if    (begin(line), end(line), IsDigit);
    auto last1  = std::find_if_not(first1     , end(line), IsDigit);
    auto first2 = std::find_if    (last1      , end(line), IsDigit);
    auto last2  = std::find_if_not(first2     , end(line), IsDigit);
    auto point  = XYPoint{};

    point.x = std::stoll(string(first1, last1));
    point.y = std::stoll(string(first2, last2));

    return point;
}

XYPoint ReadAndParseLine(ifstream &fin)
{
    auto line = string{};
    getline(fin, line);
    return ParseLine(line);
}

vector<MachineConfiguration> ReadInput(string_view filename)
{
    auto machine_configurations = vector<MachineConfiguration>{};
    for (auto fin = ifstream{ filename.data() }; fin >> std::ws;)
    {
        auto machine     = MachineConfiguration{};
        machine.button_A = ReadAndParseLine(fin);
        machine.button_B = ReadAndParseLine(fin);
        machine.target   = ReadAndParseLine(fin);

        machine_configurations.push_back(machine);
    }
    return machine_configurations;
}

pair<size_t, size_t> MinimumCostLinearEquations(
    const long long int &x1, const long long int &y1,
    const long long int &x2, const long long int &y2,
    const long long int &X, const long long int &Y
)
{
    const auto a = (x2 * Y - y2 * X) / ((y1 * x2) - (y2 * x1));
    const auto b = (X - a * x1) / x2;
    return { a, b };
}

bool IsValid(const XYPoint &kButtonA, const XYPoint &kButtonB, const XYPoint &kTarget, const long long int &kButtonAPressCount, const long long int &kButtonBPressCount)
{
    return (kButtonAPressCount * kButtonA.x + kButtonBPressCount * kButtonB.x == kTarget.x) && 
    (kButtonAPressCount * kButtonA.y + kButtonBPressCount * kButtonB.y == kTarget.y);
}

size_t Day13(const vector<MachineConfiguration> &configurations, const long long int &kFactor)
{
    constexpr auto kButtonACost = 3;
    auto sum                    = size_t{ 0 };
    for (const auto &kConfig : configurations)
    {
        const auto &[kButtonA, kButtonB, kTarget]           = kConfig;
        const auto kNewTarget                               = XYPoint{ kTarget.x + kFactor, kTarget.y + kFactor };
        const auto [kButtonAPressCount, kButtonBPressCount] = MinimumCostLinearEquations(kButtonA.x, kButtonA.y, kButtonB.x, kButtonB.y, kNewTarget.x, kNewTarget.y);
        if (IsValid(kButtonA, kButtonB, kNewTarget, kButtonAPressCount, kButtonBPressCount))
        {
            sum += kButtonAPressCount * kButtonACost + kButtonBPressCount;
        }
    }
    return sum;
}

int main(int argc, const char *args[])
{
    const auto kConfigurations = ReadInput(args[1]);

    cout << "Day 13, Part 1: " << Day13(kConfigurations, 0) << endl;
    cout << "Day 13, Part 2: " << Day13(kConfigurations, 10000000000000) << endl;

    return 0;
}
