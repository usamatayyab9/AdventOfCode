/**
 * @file day_7_bridge_repair.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compile command : g++ day_7_bridge_repair.cpp
 * @date 2025-01-05
 * @copyright Copyright (c) 2025
 * 
 */
#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <numeric>
#include <functional>

using std::array;
using std::begin;
using std::cbegin;
using std::cend;
using std::cout;
using std::end;
using std::endl;
using std::fstream;
using std::istringstream;
using std::pair;
using std::pow;
using std::string;
using std::string_view;
using std::transform;
using std::vector;

/**
 * @brief Splits a string into a vector of substrings based on a delimiter.
 * 
 * @param str The string to split.
 * @param delimiter The character to split the string by.
 * @return vector<string> A vector of substrings.
 */
vector<string> splitString(const string &str, char delimiter) {
    auto tokens      = vector<string>{};
    auto token       = string {};
    auto tokenStream = istringstream(str);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

/**
 * @brief Struct to hold calibration data.
 */
struct CalibrationData
{
    size_t      test_value;
    vector<int> numbers;
};

/**
 * @brief Reads input from a file and parses it into a vector of CalibrationData.
 * 
 * @param path The path to the input file.
 * @return vector<CalibrationData> A vector of parsed calibration data.
 */
vector<CalibrationData> ReadInput(string_view path)
{
    auto data_vec = vector<CalibrationData>{};
    auto fin      = fstream{ path.data() };
    for (auto line = string{}; getline(fin, line);)
    {
        auto pos        = line.find(':');
        auto test_value = stoull(line.substr(0, pos));
        auto numbers    = splitString(line.substr(pos + 2), ' '); //To skip the space: pos points at :, pos + 1 will be a space, pos + 2 will be first digit
        auto data       = CalibrationData{ test_value, {} };
        for (const auto &kNum : numbers) {
            data.numbers.push_back(stoi(kNum));
        }
        data_vec.push_back(data);
    }
    return data_vec;
}

/**
 * @brief Evaluates an expression based on a vector of numbers and a string of operators.
 * 
 * @param numbers The vector of numbers.
 * @param operators The string of operators.
 * @return size_t The result of the evaluated expression.
 */
size_t EvaluateExpression(const vector<int> &numbers, const string &operators)
{
    auto ShiftFactor = [](auto val) {
        auto p = size_t{ 1 };
        while (val)
        {
            val /= 10;
            p *= 10;
        }
        return p;
    };
    auto first1   = cbegin(numbers) + 1;
    auto last     = cend(numbers);
    auto first2   = cbegin(operators);
    auto sum      = static_cast<size_t>(numbers[0]);
    while (first1 != last)
    {
        if      ('+' == (*first2)) { sum += (*first1); }
        else if ('*' == (*first2)) { sum *= (*first1); }
        else if ('|' == (*first2)) { sum = sum * ShiftFactor(*first1) + (*first1); }
        ++first1;
        ++first2;
    }
    return sum;
}

/**
 * @brief Checks if an equation can be solved to match a test value using a set of numbers and valid operators.
 * 
 * @tparam N The size of the valid operators array.
 * @param test_value The test value to match.
 * @param numbers The vector of numbers.
 * @param valid_operators_array The array of valid operators.
 * @return true If the equation can be solved to match the test value.
 * @return false Otherwise.
 */
template<size_t N>
bool CanEquationBeSolved(const size_t &test_value, const vector<int> &numbers, const array<char, N> &valid_operators_array)
{
    const auto kNum = pow(size(valid_operators_array), size(numbers) - 1);
    auto operators  = string( size(numbers) - 1, '\0' );
    auto result     = false;
    for (auto i = 0; i < kNum; ++i)
    {
        auto first = rbegin(operators);
        for (auto num = i; num;)
        {
            *first = valid_operators_array[num % size(valid_operators_array)];
            num /= size(valid_operators_array);
            ++first;
        }
        if (EvaluateExpression(numbers, operators) == test_value)
        {
            result = true;
            break;
        }
    }
    return result;
}

/**
 * @brief Accumulates valid test values from calibration data that can be solved using valid operators.
 * 
 * @tparam N The size of the valid operators array.
 * @param data The vector of calibration data.
 * @param valid_operators_array The array of valid operators.
 * @return size_t The sum of valid test values.
 */
template<size_t N>
size_t AccumulateValidTestValues(const vector<CalibrationData> &data, const array<char, N> &valid_operators_array)
{
    auto sum = size_t{ 0 };
    for (const auto &[kTestValue, kNumbers] : data)
    {
        if (CanEquationBeSolved(kTestValue, kNumbers, valid_operators_array))
        {
            sum += kTestValue;
        }
    }
    return sum;
}

/**
 * @brief Main function to read input, solve the problem, and print the solutions.
 * 
 * @param argc The number of command-line arguments.
 * @param args The array of command-line arguments.
 * @return int Exit status.
 */
int main(int argc, const char* args[])
{
    const auto kTestData = ReadInput(args[1]);
    auto Day7Part1       = std::bind(AccumulateValidTestValues<2>, std::placeholders::_1, array{ '+', '*' });
    auto Day7Part2       = std::bind(AccumulateValidTestValues<3>, std::placeholders::_1, array{ '+', '*', '|' });

    cout << "Day 7 Part 1 Solution = " << Day7Part1(kTestData) << endl;
    cout << "Day 7 Part 2 Solution = " << Day7Part2(kTestData) << endl;
    return 0;
}