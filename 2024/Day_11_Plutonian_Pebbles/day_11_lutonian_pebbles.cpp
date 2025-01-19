/**
 * @file day_11_lutonian_pebbles.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compile command : g++ -std=c++17 .\day_11_lutonian_pebbles.cpp
 * @version 0.1
 * @date 2025-01-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <string_view>
#include <iterator>
#include <list>
#include <cmath>
#include <utility>
#include <algorithm>
#include <unordered_map>
#include <numeric>

using std::accumulate;
using std::begin;
using std::cbegin;
using std::cout;
using std::copy;
using std::cend;
using std::end;
using std::endl;
using std::ifstream;
using std::istream_iterator;
using std::list;
using std::log10;
using std::ostream_iterator;
using std::pow;
using std::pair;
using std::string_view;
using std::unordered_map;
using std::vector;

using StonesConfiguration_t = unordered_map<size_t, long long int>;

constexpr auto kTotalIterationsPart1 = 25;
constexpr auto kTotalIterationsPart2 = 75;

vector<int> ReadInput(string_view filename)
{
    auto fin = ifstream{ filename.data() };
    return { istream_iterator<int>{ fin }, istream_iterator<int>{} };
}

size_t DigitCount(const size_t &num)
{
    return 0 == num ? 1 : static_cast<size_t>(log10(num)) + 1;
}

pair<size_t, size_t> splitNumber(const size_t &number)
{
    const auto kDigitCount = DigitCount(number);
    const auto kHalfDigits = kDigitCount / 2;
    const auto kDivisor    = static_cast<int>(pow(10, kHalfDigits));
    const auto kLeftHalf   = number / kDivisor;
    const auto kRightHalf  = number % kDivisor;

    return { kLeftHalf, kRightHalf };
}

void ExecuteSingle(StonesConfiguration_t &original_umap)
{
    auto copy_umap = original_umap;
    for (const auto &[kStoneNumber, kStoneCount] : original_umap)
    {
        copy_umap[kStoneNumber] -= kStoneCount;
        if (0 == kStoneNumber)
        {
            copy_umap[1] += kStoneCount;    
        }
        else if (const auto kNoOfDigits = DigitCount(kStoneNumber); 0 == (kNoOfDigits % 2))
        {
            const auto [kLeftHalf, kRightHalf] = splitNumber(kStoneNumber);
            copy_umap[kLeftHalf]    += kStoneCount;
            copy_umap[kRightHalf]   += kStoneCount;
        }
        else
        {
            copy_umap[kStoneNumber * 2024] += kStoneCount;   
        }
    }
    original_umap = copy_umap;
}

void RemoveStonesZeroCount(StonesConfiguration_t &stones_umap)
{
    for (auto iter = begin(stones_umap) ; iter != end(stones_umap);)
    {
        if (iter->second <= 0)
        {
            iter = stones_umap.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

size_t CountStonesAfterNIterations(const vector<int> &stones, const int &N)
{
    auto stones_configuration = StonesConfiguration_t{ };
    //Assign initially available stones with frequency 1
    for (const auto &kStoneNumber : stones)
    {
        stones_configuration[kStoneNumber] = 1;
    }

    for (auto i = 0; i < N ;++i)
    {
        ExecuteSingle(stones_configuration);
    }

    RemoveStonesZeroCount(stones_configuration);
    
    return accumulate(cbegin(stones_configuration), cend(stones_configuration), size_t{ 0 }, [](const auto init, const auto &kKeyValuePair){
        const auto &[kStoneNumber, kStoneCount] = kKeyValuePair;
        return init + kStoneCount;
    });
}


int main(int argc, const char *args[])
{
    const auto kIntegers = ReadInput(args[1]);

    cout << "Day 11, Part 1: " << CountStonesAfterNIterations(kIntegers, kTotalIterationsPart1) << endl;
    cout << "Day 11, Part 2: " << CountStonesAfterNIterations(kIntegers, kTotalIterationsPart2) << endl;

    return 0;
}