#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string_view>
#include <string>
#include <iterator>
#include <sstream>

using std::begin;
using std::cbegin;
using std::cout;
using std::count_if;
using std::cend;
using std::end;
using std::endl;
using std::iota;
using std::ifstream;
using std::istream_iterator;
using std::size;
using std::sort;
using std::string;
using std::stringstream;
using std::string_view;
using std::vector;

vector<vector<int>> ReadInput(string_view filename)
{
    auto fin             = ifstream{ filename.data() };
    auto reports_vector  = vector<vector<int>>{};
    for (auto line = string{}; std::getline(fin, line) ;)
    {
        auto strstream = stringstream{ line };
        reports_vector.push_back({ istream_iterator<int>{ strstream }, istream_iterator<int>{} });
    }
    return reports_vector;
}

bool IsSafe(const vector<int> &report)
{
    auto is_decreasing = true;
    auto is_increasing = true;
    for (auto idx = size_t{ 1 }; idx < report.size() ;++idx)
    {
        const auto kDiff = report[idx - 1] - report[idx];
        is_decreasing   &= kDiff >= 1 && kDiff <= 3;

        const auto kDiff2 = report[idx] - report[idx - 1];
        is_increasing   &= kDiff2 >= 1 && kDiff2 <= 3;
    }
    return is_decreasing || is_increasing;
}

bool IsSafe_Part2(const vector<int> &report, const size_t &kIndexToSkip)
{
    auto indices_vec   = vector<size_t>(size(report));
    iota(begin(indices_vec), end(indices_vec), 0);
    indices_vec.erase(begin(indices_vec) + kIndexToSkip);
    
    auto is_decreasing = true;
    auto is_increasing = true;
    for (auto idx = size_t{ 1 }; idx < indices_vec.size() ;++idx)
    {
        const auto kPreviousIdx = indices_vec[idx - 1];
        const auto kNextIdx     = indices_vec[idx];
        const auto kPrevious    = report[kPreviousIdx];
        const auto kNext        = report[kNextIdx]; 

        const auto kDiff = kPrevious - kNext;
        is_decreasing   &= kDiff >= 1 && kDiff <= 3;

        const auto kDiff2 = kNext - kPrevious;
        is_increasing   &= kDiff2 >= 1 && kDiff2 <= 3;
    }
    return is_decreasing || is_increasing;
}

bool IsSafe_Part2(const vector<int> &report)
{
    auto is_safe = false;
    for (auto idx = size_t{ 0 } ; idx < report.size() && !is_safe ; ++idx)
    {
        if (IsSafe_Part2(report, idx))
        {
            is_safe = true;
        }
    }
    return is_safe;
}

int Day_1_Part_1(const vector<vector<int>> &reports)
{
    return count_if(cbegin(reports), cend(reports), IsSafe);
}

int Day_1_Part_2(const vector<vector<int>> &reports)
{
    return count_if(cbegin(reports), cend(reports), [](const auto &report){
        return IsSafe(report) || IsSafe_Part2(report);
    });

}

int main(int argc, const char *args[])
{
    const auto kReports = ReadInput(args[1]);
    cout << "Day 1, Part 1: " << Day_1_Part_1(kReports) << endl;
    cout << "Day 1, Part 2: " << Day_1_Part_2(kReports) << endl;

    return 0;
}