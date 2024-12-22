#include <iostream>
#include <fstream>
#include <iterator>
#include <numeric>
#include <string>

using namespace std;

int main(int argc, const char *args[])
{

    auto fin = ifstream{ args[1] };
    auto sum = std::accumulate(istream_iterator<string>{ fin }, istream_iterator<string>{}, 0,
        [](const auto &sum_so_far, const string &line){
            const auto kFirstDigitPos = line.find_first_of("1234567890");
            const auto kLastDigitPos  = line.find_last_of("1234567890");
            const auto kFirstDigit    = line[kFirstDigitPos] - '0';
            const auto kLastDigit     = line[kLastDigitPos] - '0';
            const auto kNumber        = (kFirstDigit * 10) + kLastDigit;
            return sum_so_far + kNumber;
        }
    );
    cout << "Sum: " << sum << endl;

    return 0;
}