/**
 * @file day_1_historian_hysteria.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compile command : g++ -std=c++17 .\day_1_historian_hysteria.cpp
 * @version 0.1
 * @date 2024-12-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string_view>
#include <iterator>
#include <functional>
#include <unordered_map>

using std::accumulate;
using std::begin;
using std::cbegin;
using std::cout;
using std::cend;
using std::end;
using std::endl;
using std::ifstream;
using std::inner_product;
using std::pair;
using std::sort;
using std::string_view;
using std::unordered_map;
using std::vector;

pair<vector<long>, vector<long>> ReadInput(string_view filename)
{
    auto fin = ifstream{ filename.data() };
    auto v1  = vector<long>{};
    auto v2  = vector<long>{}; 
    for (auto a = long{ 0 }, b = long{ 0 }; fin >> a >> b ;)
    {
        v1.push_back(a);
        v2.push_back(b);
    }
    return { v1, v2 };
}

long Day_1_Part_1(vector<long> vec1, vector<long> vec2)
{
    sort(begin(vec1), end(vec1));
    sort(begin(vec2), end(vec2));
    return inner_product(cbegin(vec1), cend(vec1), cbegin(vec2), 0, std::plus{}, [](const auto &a, const auto &b){
        return std::abs(a - b);
    });
}

long Day_1_Part_2(const vector<long> &vec1, const vector<long> &vec2)
{
    auto frequency_umap = unordered_map<long, long>{};
    for (const auto &kValue : vec2) {
        if (frequency_umap.count(kValue) >= 1)
        {
            ++frequency_umap[kValue];
        }
        else
        {
            frequency_umap[kValue] = 1;
        }
    }
    return accumulate(cbegin(vec1), cend(vec1), long{ 0 }, [&frequency_umap](const auto &init, const auto &kCurrentValue){
        auto sum = long{ 0 };
        if (frequency_umap.count(kCurrentValue) >= 1)
        {
            sum = kCurrentValue * frequency_umap[kCurrentValue];
        }
        return sum + init;
    });

}

int main(int argc, const char *args[])
{
    const auto [kVec1, kVec2] = ReadInput(args[1]);
    cout << "Day 1, Part 1: " << Day_1_Part_1(kVec1, kVec2) << endl;
    cout << "Day 1, Part 2: " << Day_1_Part_2(kVec1, kVec2) << endl;
    return 0;
}