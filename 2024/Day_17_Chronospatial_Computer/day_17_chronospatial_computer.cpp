/**
 * @file day_17_chronospatial_computer.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compile command : g++ -std=c++17 .\day_17_chronospatial_computer.cpp
 * 
 * This problem involves understanding what program does. For this I manually decoded the program. After going through my input program, I realized that
 * initial values of register B and C are irrelevant as they are re-assigned in every iteration. And register A is divided by 8 in every iteration. 
 * 
 * Part 1 was very straightforward, it just involves simulating the program.
 * @function vector<int> Day17Part1(const size_t &seed) : This function simulates the program as per my puzzle input, and returns the output of program as
 * vector of integers.
 * The default initial value of register A used for part 1 is passed as argument in main function.
 * 
 * Part 2:
 * I reverse enginned the register A value.
 * In my case, program was : 2,4,1,5,7,5,0,3,4,0,1,6,5,5,3,0
 * Task was to find initial value for register A such that output is same as input program and the value is lowest possible.
 * 
 * -- If we take last digit of program '0':
 *      We have to find the value of register A such that output of program is 0. For my input it appears there is only one value of register A which is '3'.
 * 
 * -- If we take 2nd last digit of program '3':
 *      We have to find the value of register A such when we simulate the program we get 3 as output. And when we divide it by 8 we go to above step i.e. 0.
 *      There are multiple values which when divided by 8 gives 3, they are:
 *      24, 25, 26, 27, 28, 29, 30, 31, 32. Among these we take those that produce 3 as output. Which are: 24, 25, 29, 31.
 * 
 * -- Similarly we keep going rest of the digits of program. In the end we will have multiple values of register A which produce same output as input program.
 *      We will take the minimum value of register A which produces same output as input program.
 * 
 * Here are some few more iterations:
 * Program: 2,4,1,5,7,5,0,3,4,0,1,6,5,5,3,0
 * Iteration 0; Target 0: 3
 * Iteration 1; Target 3: 24, 25, 29, 31
 * Iteration 2; Target 5: 192, 196, 198, 199, 200, 249
 * Iteration 3; Target 5: 1538, 1540, 1542, 1543, 1570, 1572, 1575, 1586, 1588, 1594, 1602, 1604, 1606, 1607, 1992
 * Iteration 4; Target 6: 12691, 12704, 12707, 15937
 * Iteration 5; Target 1: 101532, 101660, 127500
 * Iteration 6; Target 0: 812258, 813282, 1020000
 * Iteration 7; Target 4: 6498067, 6506259, 8160003
 * Iteration 8; Target 3: 51984537, 51984541, 51984543, 52050073, 52050077, 52050079, 65280024, 65280025, 65280029, 65280031
 * Iteration 8; Target 0: 415876298, 415876330, 415876334, 415876346, 416400586, 416400618, 416400622, 416400634, 522240194, 522240202, 522240234, 522240238, 522240250 
 * 
 * @date 2025-03-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <iostream>
#include <cmath>
#include <limits>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <string_view>

using std::binary_search;
using std::copy;
using std::cout;
using std::endl;
using std::ostream_iterator;
using std::pow;
using std::string;
using std::string_view;
using std::vector;

vector<int> Day17Part1(const size_t &Seed)
{
    auto A = size_t{ Seed };
    auto B = size_t{ 0 };
    auto C = size_t{ 0 };
    auto vec = vector<int>{};
    while (0 != A)
    {
        B = A % 8;
        B = B ^ 5;
        C = A / pow(2, B);
        A = A / pow(2, 3);
        B = B ^ C;
        B = B ^ 6;

        vec.push_back(B % 8);
    }
    return vec;
}

size_t CalculateRegisterB(size_t A)
{
    auto B = A % 8;
    B = B ^ 5;
    auto C = static_cast<size_t>(A / pow(2, B));
    A = A / pow(2, 3);
    B = B ^ C;
    B = B ^ 6;

    return B;
}

long long int Day17Part2(const vector<int> &targets, vector<long long int> previous_values)
{
    auto vec = vector<long long int>{};
    for (const auto &target : targets)
    {
        vec.clear();
        for (const auto &val : previous_values)
        {
            for (auto start = val * 8, end = 8 * (val + 1); start < end; ++start)
            {
                //Binary Search is just to avoid duplicate values.
                if ((CalculateRegisterB(start) % 8 == target) && binary_search(cbegin(previous_values), cend(previous_values), start / 8))
                {
                    vec.push_back(start);
                }
            }
        }
        previous_values = vec;
    }

    return *std::min_element(cbegin(previous_values), cend(previous_values));
}



int main(int argc, char* argv[])
{
    const auto kOutputVec = Day17Part1(46187030);
    cout << "Day 17 Part 1: ";
    copy(cbegin(kOutputVec), cend(kOutputVec) - 1, ostream_iterator<int>(cout, ","));
    cout << kOutputVec.back() << "\n";

    const auto kTargetsVec             = vector<int>{ 3, 5, 5, 6, 1, 0, 4, 3, 0, 5, 7, 5, 1, 4, 2 };
    const auto kInitalVecForLastDigit  = vector<long long int>{ 3 };
    cout << "Day 17, Part 2: " << Day17Part2(kTargetsVec, kInitalVecForLastDigit) << endl;


    return 0;
}