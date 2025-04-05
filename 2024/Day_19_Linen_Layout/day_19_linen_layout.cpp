/**
 * @file day_19_linen_layout.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command:
 * g++ -std=c++17 -o day_19_linen_layout day_19_linen_layout.cpp
 * 
 * @date 2025-04-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <utility>
#include <string_view>
#include <sstream>
#include <iterator>
#include <numeric>
#include <functional>
#include <queue>
#include <unordered_map>

#include <limits>

using std::accumulate;
using std::cout;
using std::cbegin;
using std::cend;
using std::endl;
using std::ifstream;
using std::istream_iterator;
using std::getline;
using std::string;
using std::pair;
using std::queue;
using std::stringstream;
using std::string_view;
using std::unordered_map;
using std::vector;

pair<vector<string>, vector<string>> ReadInput(const string& filename)
{
    auto p = pair<vector<string>, vector<string>>{};
    if (auto fin = ifstream{ filename.data() }; fin)
    {
        auto str = string{};
        getline(fin, str);
        str.erase(remove(begin(str), end(str), ' '), end(str));
        for (auto ss = stringstream{ str }; getline(ss, str, ','); )
        {
            p.first.push_back(str);
        }
        p.second.assign(istream_iterator<string>{ fin }, istream_iterator<string>{});
    }
    return p;
}

size_t CountAllPossibleWays(const string &kTargetDesign, const size_t &kIdx, const vector<string> &kAvailablePatterns,
    unordered_map<size_t, size_t> &dp_table) 
{    
    if (kIdx == size(kTargetDesign)) {
        return 1;
    }

    if (dp_table.find(kIdx) != cend(dp_table)) {
        return dp_table[kIdx];
    }

    auto count                = size_t{ 0 };
    const auto kRemainingSize = size(kTargetDesign) - kIdx;
    for (const auto &kPattern : kAvailablePatterns)
    {
        if (size(kPattern) <= kRemainingSize && std::equal(cbegin(kPattern), cend(kPattern), cbegin(kTargetDesign) + kIdx))
        {
            count += CountAllPossibleWays(kTargetDesign, kIdx + size(kPattern), kAvailablePatterns, dp_table);
        }
    }

    dp_table[kIdx] = count;

    return count;
}

int Day19Part1(const vector<string>& kAvailablePatterns, const vector<string>& kDesiredDesigns)
{
    return accumulate(cbegin(kDesiredDesigns), cend(kDesiredDesigns), 0,
        [&](const auto &acc, const auto &kTargetDesign)
        {
            auto dp_table          = unordered_map<size_t, size_t>{};
            const auto kIsPossible = CountAllPossibleWays(kTargetDesign, 0, kAvailablePatterns, dp_table) > 0;
            return acc + kIsPossible;
        }
    );
}

size_t Day19Part2(const vector<string>& kAvailablePatterns, const vector<string>& kDesiredDesigns)
{
    auto sum      = size_t{ 0 };
    auto dp_table = unordered_map<size_t, size_t>{};
    for (const auto &kTargetDesign : kDesiredDesigns)
    {
        dp_table.clear();
        sum += CountAllPossibleWays(kTargetDesign, 0, kAvailablePatterns, dp_table);
    }
    return sum;
}

int main(const int argc, const char *args[])
{
    const auto [kAvailablePatterns, kDesiredDesigns] = ReadInput(args[1]);

    cout << "Day 19, Part 1: " << Day19Part1(kAvailablePatterns, kDesiredDesigns) << endl;
    cout << "Day 19, Part 2: " << Day19Part2(kAvailablePatterns, kDesiredDesigns) << endl;

    return 0;
}