/**
 * @file sonar_sweep.cpp
 * @author Usama (usamatayyab9@gmail.com)
 * @brief 
 * This code is the solution of @link https://adventofcode.com/2021/day/1 @endlink
 * The following is compiled on g++ version 9.4.0. 
 * Compilation command : g++ -std=c++17 sonar_sweep.cpp
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
#include <string_view>

using namespace std;

/**
 * @brief 
 * Algorithm:
 * let N = size(vec)
 * let sum = 0
 * for i = 1 ... N 
 *   if vec[i] > vec[i -1]
 *      sum += 1
 * @param vec 
 * @return int
 * count of values greater than value at previous index.
 * If input vector is of size 1 then return value is 0
 */
auto CountValueIncrease(const vector<int> &vec)
{
    return inner_product(cbegin(vec) + 1, cend(vec),
        cbegin(vec),
        0,
        std::plus{},
        std::greater{}
    );
}

auto Part2(const vector<int> &vec)
{
    auto window = size_t{ 3 };
    //precompute the sum of first {window in this case 3} values
    auto sum                    = accumulate(vec.cbegin(), vec.cbegin() + window, 0);
    auto window_accumulated_vec = vector<int>{sum};
    transform(std::cbegin(vec) + window, std::cend(vec),
        std::cbegin(vec),
        back_inserter(window_accumulated_vec),
        [&sum](const int &forward, const int &backward){
            sum -= backward;
            sum += forward;
            return sum;
        }
    );
    return CountValueIncrease(window_accumulated_vec);
}

/**
 * @brief 
 * opens the file represented by @arg filename.
 * Read its contents in a vector of integers and
 * return the resultant vector
 * @param filename
 * @return vector<int> 
 */
vector<int> ReadInput(string_view filename)
{
    auto fin = ifstream{filename.data()};
    return {istream_iterator<int>{fin}, istream_iterator<int>{}};
}

int main(int argc, const char *args[])
{
    //Read the file passed as command line argument
    const auto kVector = ReadInput(args[1]);
    cout << "Day 1: Sonar Sweep\n"; 
    cout << "Part 1 : " << CountValueIncrease(kVector) << endl;
    cout << "Part 2 : " << Part2(kVector) << endl;
    return 0;
}