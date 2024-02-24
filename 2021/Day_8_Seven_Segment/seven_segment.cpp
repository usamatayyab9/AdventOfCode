/**
 * @file seven_segment.cpp
 * @author Usama Tayyab (usamatayyab@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 ./seven_segment.cpp
 * Compiled with gcc 9.4.0
 * This code is the solution of @link https://adventofcode.com/2021/day/8 @endlink
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iterator>
#include <array>
#include <iomanip>
#include <utility>
#include <numeric>
#include <unordered_map>
#include <string_view>
#include <bitset>
#include <charconv>

using std::ifstream;
using std::cout;
using std::string;
using std::vector;
using std::stringstream;
using std::istream_iterator;
using std::ostream_iterator;
using std::array;
using std::find;
using std::cbegin;
using std::cend;
using std::size;
using std::endl;
using std::pair;
using std::string_view;
using std::bitset;
using std::from_chars;

void print(const auto &container, const char &delim = ' ')
{
    for (const auto &elem : container) { std::cout << elem << delim; }
    std::cout << std::endl;
}

struct SignalsAndDigits
{
    array<string, 10> unique_signals;
    array<string, 4> digits;
    void print() const
    {
        copy(cbegin(unique_signals), cend(unique_signals), ostream_iterator<string>{std::cout, " "});
        std::cout << "| ";
        copy(cbegin(digits), cend(digits), ostream_iterator<string>{std::cout, " "});
        std::cout << std::endl;
    }
    static SignalsAndDigits fromString(const string &str)
    {
        auto ss      = stringstream{str};
        auto signals = array<string, 10>{};
        auto digits  = array<string, 4>{};
        std::copy_n(istream_iterator<string>{ss}, size(signals), begin(signals));
        //Reason for next is to skip | in input string
        std::copy_n(std::next(istream_iterator<string>{ss}), size(digits), begin(digits));
        return {signals, digits};
    }
};

vector<char> GetUnCommonElements(string s1, string s2)
{
    std::sort(begin(s1), end(s1));
    std::sort(begin(s2), end(s2));
    // cout << "Calculating uncommon elements between " << std::quoted(s1) << " and " << std::quoted(s2) << endl;
    auto uncommon_elements = vector<char>{};
    std::set_symmetric_difference(
        cbegin(s1), cend(s1),
        cbegin(s2), cend(s2),
        std::back_inserter(uncommon_elements));
    // print(uncommon_elements);
    return uncommon_elements;
}

pair<char, char> GetUpperBottomRightFromSixAndOne(const string &one, const string &six)
{
    auto upper_rt  = '\0';
    auto bottom_rt = '\0';
    auto pos1      = six.find(one[0]);
    if (pos1 == string::npos) { upper_rt = one[0]; bottom_rt = one[1]; }
    else                      { upper_rt = one[1]; bottom_rt = one[0]; }
    return {upper_rt, bottom_rt};
}

struct SegmentTemplate
{
    char upper;
    char upper_lt;
    char upper_rt;
    char mid;
    char bottom_lt;
    char bottom_rt;
    char bottom;
    void print() const
    {
        cout
        << "upper:     " << upper     << endl
        << "upper_lt:  " << upper_lt  << endl
        << "upper_rt:  " << upper_rt  << endl
        << "mid:       " << mid       << endl
        << "bottom_lt: " << bottom_lt << endl
        << "bottom_rt: " << bottom_rt << endl
        << "bottom:    " << bottom    << endl;
    }
};

SegmentTemplate MakeTemplateFromSignals(const array<string, 10> &signals)
{
    // cout << "Processing signals...\n";
    // print(signals);
    //Find the signal represented by 1. 1 can represented by using only two segments.
    const auto &one   = *find_if(cbegin(signals), cend(signals), [](const string &str) { return size(str) == 2; });
    //Find the signal represented by 7. 7 can represented by using only three segments.
    const auto &seven = *find_if(cbegin(signals), cend(signals), [](const string &str) { return size(str) == 3; });
    //Find the signal represented by 4. 4 can represented by using onyl four segments.
    const auto &four  = *find_if(cbegin(signals), cend(signals), [](const string &str) { return size(str) == 4; });
    // cout << "one : "   << one   << endl;
    // cout << "seven : " << seven << endl;
    // cout << "four : "  << four  << endl;
    
    const auto upper  = GetUnCommonElements(one, seven).front();
    // cout << "upper segment: " << upper << endl;
    //possible_ult_mid will contain two values. These two value belong to upper-left and mid segment.
    const auto possible_upperlt_mid = GetUnCommonElements(one, four);
    // cout << "possible values for upper-left and mid:" << endl;  print(possible_upperlt_mid); 
    
    //Only 0, 6 and 9 uses six length segments.
    auto all_six_length_signals = vector<string>(3);
    copy_if(cbegin(signals), cend(signals), begin(all_six_length_signals), [](const auto &signal) { return size(signal) == 6; });
    // cout << "all signals with 6 lenght: "; print(all_six_length_signals);

    auto six_pos = find_if(cbegin(all_six_length_signals), cend(all_six_length_signals), [&one](const auto &signal){
        auto pos1 = signal.find(one[0]);
        auto pos2 = signal.find(one[1]);
        return !((pos1 != string::npos) && (pos2 != string::npos));
    });
    const auto six = *six_pos;
    // cout << "six : "  << six  << endl;
    all_six_length_signals.erase(six_pos);
    const auto [upper_rt, bottom_rt] = GetUpperBottomRightFromSixAndOne(one, six);
    // cout << "upper-rt segment:  " << upper_rt  << endl;
    // cout << "bottom-rt segment: " << bottom_rt << endl;

    // cout << "0 and 9 signals: "; print(all_six_length_signals);
    //uncommon values among 0 and 9 segments will give info about mid and bottom-left
    //After that we can compute upper-left and bottom too
    auto uncommon_elems = GetUnCommonElements(all_six_length_signals[0], all_six_length_signals[1]);
    // print(uncommon_elems);
    const auto [upper_lt, mid, bottom_lt] = [&uncommon_elems, &possible_upperlt_mid](){
        auto upper_lt  = '\0';
        auto mid       = '\0';
        auto bottom_lt = '\0';
        auto pos = find(cbegin(uncommon_elems), cend(uncommon_elems), possible_upperlt_mid[0]);
        if (pos != cend(uncommon_elems))
        {
            mid      = possible_upperlt_mid[0];
            upper_lt = possible_upperlt_mid[1];
        }
        else
        {
            mid      = possible_upperlt_mid[1];
            upper_lt = possible_upperlt_mid[0];
            pos      = find(cbegin(uncommon_elems), cend(uncommon_elems), possible_upperlt_mid[1]);
        }
        // cout << "remaining element: " << *pos << endl;
        uncommon_elems.erase(pos);
        // print(uncommon_elems);
        bottom_lt = uncommon_elems.front();
        return std::make_tuple(upper_lt, mid, bottom_lt);
    }();
    // cout << "upper-left: "  << upper_lt  << endl;
    // cout << "mid: "         << mid       << endl;
    // cout << "bottom-left: " << bottom_lt << endl;
    
    //Now all segments are computed except bottom, which can be caluclated
    //by finding the missing char
    const auto &eight  = *find_if(cbegin(signals), cend(signals), [](const string &str) { return size(str) == 7; });
    const char bottom = std::accumulate(cbegin(eight), cend(eight), 0) - (upper + upper_rt + upper_lt + bottom_rt + mid + bottom_lt);
    // cout << "bottom: " << bottom << endl;
    return {upper, upper_lt, upper_rt, mid, bottom_lt, bottom_rt, bottom};
}

char CalculateDigit(const SegmentTemplate &seg_template, const string &digits)
{
    // cout << "digits: " << digits << endl;
    // seg_template.print();
    auto sz = size(digits);
    if      (sz == 2) { return '1'; }
    else if (sz == 4) { return '4'; }
    else if (sz == 3) { return '7'; }
    else if (sz == 7) { return '8'; }
    else
    {
        static const auto template_map = std::unordered_map<std::bitset<7>, char> ({
            {bitset<7>("1110111"), '0'},
            {bitset<7>("1011101"), '2'},
            {bitset<7>("1011011"), '3'},
            {bitset<7>("1101011"), '5'},
            {bitset<7>("1101111"), '6'},
            {bitset<7>("1111011"), '9'}
        });
        auto str = string{};
        auto bs = bitset<7>{};
        for (const auto &d : digits)
        {
            if (d == seg_template.upper)     { bs[6] = 1; }
            if (d == seg_template.upper_lt)  { bs[5] = 1; }
            if (d == seg_template.upper_rt)  { bs[4] = 1; }
            if (d == seg_template.mid)       { bs[3] = 1; }
            if (d == seg_template.bottom_lt) { bs[2] = 1; }
            if (d == seg_template.bottom_rt) { bs[1] = 1; }
            if (d == seg_template.bottom)    { bs[0] = 1; }
        }
        // cout << bs << endl;
        return template_map.at(bs);
    }
}

int DigitsToInt(const SegmentTemplate &seg_template, const array<string, 4> &encoded_digits)
{
    auto digits = std::array<char, encoded_digits.size()>{};
    std:transform(cbegin(encoded_digits), cend(encoded_digits), begin(digits), [&seg_template](const auto &str){ return CalculateDigit(seg_template, str); });
    auto i = 0;
    from_chars(data(digits), data(digits) + size(digits), i);
    return i;
}

int main(int argc, const char *args[])
{
    auto fin = std::ifstream{args[1]};
    auto temp_str = std::string{};
    auto vec = vector<int>{};
    while (getline(fin, temp_str))
    {
        const auto signal_and_digits = SignalsAndDigits::fromString(temp_str);
        auto segment_template = MakeTemplateFromSignals(signal_and_digits.unique_signals);
        auto num = DigitsToInt(segment_template, signal_and_digits.digits);
        vec.push_back(num);
        // cout << "num: " << num << endl;
    }
    cout << "TOTAL SUM = " << std::accumulate(cbegin(vec), cend(vec), 0);
    return 0;
}