/**
 * @file lantern.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * The following is compiled on g++ version 9.4.0.
 * Compilation command : g++ -std=c++17 ./lantern.cpp
 *  This code is the solution of @link https://adventofcode.com/2021/day/1 @endlink
 *  
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <algorithm>
#include <numeric>
#include <valarray>
#include <fstream>
#include <vector>
#include <charconv>
#include <list>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <map>

using namespace std;
vector<int> readInput(const string  &filename)
{
    auto fin      = ifstream{filename};
    auto vec      = vector<int>{};
    auto temp_str = string{};
    auto temp_i   = 0;
    while (getline(fin, temp_str, ','))
    {
        from_chars(data(temp_str), data(temp_str) + size(temp_str), temp_i);
        vec.push_back(temp_i);
    }
    return vec;
}

struct LanterFishCalculator
{
    size_t operator()(int initial_timer, int days)
    {
        return LanterFishUtility_REC(initial_timer, days);
    }
    private:
    size_t LanterFishUtility_REC(int timer, int days)
    {
        //if answer is already computed, then return saved answer
        if (dp_map.find({timer, days}) != dp_map.cend()) { return dp_map[{timer, days}]; }
        auto total_fishes = size_t{0};
        if (days == 0)     { total_fishes = 0; }
        if (days <= timer) { total_fishes = 1; }//If no of days are less than fish timer, then no new fishes will be generated
        else
        {
            total_fishes = 1;
            for (auto new_fish_day = timer + 1; new_fish_day <= days ;new_fish_day += (6 + 1))
            {
                total_fishes += LanterFishUtility_REC(8, days - new_fish_day);
            }
        }
        dp_map[{timer, days}] = total_fishes;
        return total_fishes;
    }
    static map<pair<int, int>, size_t> dp_map;
};
map<pair<int, int>, size_t> LanterFishCalculator::dp_map;

auto CalculateFishCountAfter_N_Days(const vector<int> &initial_timers, const int &N)
{
    auto lanter_fish_calculator = LanterFishCalculator{};
    auto total_fishes           = size_t{ 0 };
    //Calculate how fishes will be available after {N} days.
    //NOTE: LanterFishCalculator uses memoization technique to store previous results
    //So many of the times same computations will not be performed again.
    for (const auto &timer : initial_timers) { total_fishes += lanter_fish_calculator(timer, N); }
    return total_fishes;
}

int main(int argc, const char *args[])
{
    //Read data from file
    cout << "data : " << endl;
    auto initial_timers = readInput(args[1]);
    
    cout << "Part 1(After 80 days): " << CalculateFishCountAfter_N_Days(initial_timers, 80) << endl;
    cout << "Part 1(After 256 days): " << CalculateFishCountAfter_N_Days(initial_timers, 256) << endl;
    return 0;
}