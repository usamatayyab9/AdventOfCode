/**
 * @file bin_diagnose.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * This code is the solution of @link https://adventofcode.com/2021/day/3 @endlink
 * The following is compiled on g++ version 9.4.0. 
 * Compilation command : g++ -std=c++17 bin_diagnose.cpp
 * The program takes input the name of file as command line argument.
 * For example if a.out is the binary obtained by compiling this program,
 * then it should be run as:
 *                  a.out test.txt
 *                  a.out input.txt
 * @date 2022-01-11
 * @copyright Copyright (c) 2022
*/
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <vector>
#include <numeric>
#include <unordered_map>

using namespace std;

vector<bool> getSlice(const vector<bool> &vec, size_t start, size_t jump)
{
    auto result = vector<bool>{};
    result.reserve(vec.size() / jump);
    for (; start < vec.size(); start += jump)
    {
        result.push_back(vec[start]);
    }
    return result;
}

bool mostCommon(const vector<bool> &vec)
{
    auto umap = std::unordered_map<bool, size_t>{};
    for (const auto &bit : vec)          { ++umap[bit]; }
    if  (umap.at(false) > umap.at(true)) { return false; }
    return true;
}

string vecToBitStr(const vector<bool> &vec)
{
    auto str = string(vec.size(), '\0');
    transform(cbegin(vec), cend(vec), begin(str), [](const bool &bit) { return '0' + bit; });
    return str;
}

vector<vector<bool>> readInput(const string &filename)
{
    auto fin = ifstream{filename};
    auto numbers = vector<vector<bool>>{};
    transform(
        istream_iterator<string>{fin},
            istream_iterator<string>{},
                back_inserter(numbers), 
                    [](const string &str)
                    {
                        auto vec = vector<bool>(str.size());
                        transform(cbegin(str), cend(str), begin(vec), [](const char &ch) {
                            return ch - '0'; 
                        });
                        return move(vec);
                    }
    );
    return numbers;
}

vector<bool> GetColumn(const vector<vector<bool>> &nums, size_t idx)
{
    auto result = vector<bool>{};
    result.reserve(nums.size());
    transform(cbegin(nums), cend(nums), back_inserter(result), [idx](const auto &v)
    {
        return v[idx];
    });
    return result;
}

void print_seq_cont(auto &cont)
{
    copy(cbegin(cont), cend(cont), ostream_iterator<bool>{cout, ""}); cout << endl;
}

auto BitVecToInt(const vector<bool> &bit_vec)
{
    return stoi(vecToBitStr(bit_vec), nullptr, 2);
}

pair<size_t, size_t> CountDual(const vector<bool> &vec)
{
    auto one_cnt = size_t{0};
    auto zero_cnt = size_t{0};
    for (const auto &bit : vec) {
        one_cnt += (bit == true);
        zero_cnt += (bit == false);
    }
    return {one_cnt, zero_cnt};
}

void print(const vector<vector<bool>> &mat)
{
    for (const auto &vec : mat)
    {
        print_seq_cont(vec);
    }
}

struct Part2
{
    Part2(const vector<vector<bool>> arg):
        numbers{arg}
    {}
    int LifeSupportRating()
    {
        return O2GeneratorRating() * CO2ScrubberRating();
    }
    int O2GeneratorRating()
    {
        auto nums = numbers;
        const auto cols = size(nums[0]);
        for (auto i = size_t{0}; nums.size() != 1 ;++i)
        {
            auto column = GetColumn(nums, i);
            auto [one_cnt, zero_cnt] = CountDual(column);
            auto most_common = 1;
            if (one_cnt >= zero_cnt) { most_common = 1; }
            else                     { most_common = 0; }
            auto end_iter = remove_if(begin(nums), end(nums), [&i, &most_common](const auto &vec){
                return vec[i] != most_common;
            });
            nums.erase(end_iter, cend(nums));
        }
        return BitVecToInt(nums.front());
    }

    int CO2ScrubberRating()
    {
        auto nums = numbers;
        const auto cols = size(nums[0]);
        for (auto i = size_t{0}; nums.size() != 1 ;++i)
        {
            auto column = GetColumn(nums, i);
            auto [one_cnt, zero_cnt] = CountDual(column);
            auto least_common = 0;
            if (zero_cnt <= one_cnt) { least_common = 0; }
            else                     { least_common = 1; }
            auto end_iter = remove_if(begin(nums), end(nums), [&i, &least_common](const auto &vec){
                return vec[i] != least_common;
            });
            nums.erase(end_iter, cend(nums));
        }
        return BitVecToInt(nums.front());
    }
    vector<vector<bool>> Transpose()
    {
        const auto rows = size(numbers);
        const auto cols = size(numbers[0]);
        auto numbersT = vector<vector<bool>>(cols, vector<bool>(rows));
        for (auto i = size_t{0}; i < rows ;++i) {
            for (auto j = size_t{0}; j < cols; ++j) {
                numbersT[j][i] = numbers[i][j];
            }
        }
        return numbersT;
    }
    const vector<vector<bool>> numbers;
};

int main(int argc, const char *args[])
{
    cout << "reading input..." << args[1] << endl;
    auto numbers = readInput(args[1]);
    cout << "total binary numbers read..." << numbers.size() << endl;
    auto number_len = numbers[0].size();
    cout << "each number is of length: " << number_len << endl;

    // for (auto &num : numbers) { print(num); }

    // auto most_common_bits = vector<bool>{};
    // most_common_bits.reserve(number_len);
    // for (auto i = 0; i < number_len ;++i)
    // {
        // auto num = GetColumn(numbers, i);
        // print(num);
        // most_common_bits.push_back(mostCommon(GetColumn(numbers, i)));
    // }
    // auto a = BitVecToInt(most_common_bits);
    // auto least_common_bits = most_common_bits;
    // least_common_bits.flip();
    // auto b = BitVecToInt(least_common_bits);
    // cout << a * b;


    {
        auto p2 = Part2{numbers};
        cout << p2.LifeSupportRating() << std::endl;

    }
    return 0;
}