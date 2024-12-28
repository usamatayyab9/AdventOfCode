#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <string_view>
#include <string>
#include <iterator>
#include <sstream>
#include <regex>

using std::begin;
using std::cbegin;
using std::cout;
using std::cend;
using std::end;
using std::endl;
using std::ifstream;
using std::istreambuf_iterator;
using std::size;
using std::string;
using std::string_view;
using std::vector;

string ReadInput(string_view filename)
{
    auto fin = ifstream{ filename.data() };
    return string{ istreambuf_iterator<char>{ fin }, istreambuf_iterator<char>{} };
}

long Day3Part1(const string &kInput)
{
    const auto kRegex = std::regex{ "mul\\((\\d{1,3}),(\\d{1,3})\\)" };
    auto sum          = long{ 0 };
    for (auto it = std::sregex_iterator{ cbegin(kInput), cend(kInput), kRegex }; it != std::sregex_iterator{}; ++it)
    {
        const auto kMatch = *it;
        sum += std::stol(kMatch[1].str()) * std::stol(kMatch[2].str());
    }
    return sum;
}

long Day3Part2(const string &kInput)
{
    const auto kRegex   = std::regex{ "do(?!n't)\\b|\\bdon't\\b|mul\\((\\d{1,3}),(\\d{1,3})\\)" };
    auto sum            = long{ 0 };
    auto do_multiply    = true;
    for (auto it = std::sregex_iterator{ cbegin(kInput), cend(kInput), kRegex }; it != std::sregex_iterator{}; ++it)
    {
        if (const auto kMatchedStr = it->str(); "do" == kMatchedStr)
        {
            do_multiply = true;
        }
        else if ("don't" == kMatchedStr)
        {
            do_multiply = false;
        }
        else if (string::npos != kMatchedStr.find("mul"))
        {
            const auto kMatch = *it;
            sum += do_multiply ? std::stol(kMatch[1].str()) * std::stol(kMatch[2].str()) : 0;
        }
    }
    return sum;
}

int main(int argc, const char *args[])
{
    const auto kInput = ReadInput(args[1]);
    cout << "Day 3, Part 1: " << Day3Part1(kInput) << endl;
    cout << "Day 3, Part 2: " << Day3Part2(kInput) << endl;
    return 0;
}