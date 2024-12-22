#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <tuple>

using namespace std;

bool TestAndUpdateCycleToProcess(const int &cycles_so_far, int &cycle_to_process)
{
    const auto kShouldProcessCycle = (cycle_to_process == cycles_so_far);
    if (kShouldProcessCycle)
    {
        cycle_to_process += 40;
    }
    return kShouldProcessCycle;
}

int main(int argc, const char *args[])
{
    auto fin                = ifstream{ args[1] };
    auto str                = string{};
    auto cycles             = 0;
    auto X                  = 1;
    auto cycle_to_process   = 20;
    auto sum_of_strngths    = 0;
    
    for (auto [value_to_add, ii] = tuple{ 0, 0 }; fin >> str;)
    {
        // cout << str << ", cycles: " << cycles << ", X: " << X << endl;
        if (str.find("noop") != str.npos)
        {
            value_to_add = 0;
            ii           = 1;
        }
        else if (str.find("addx") != str.npos)
        {
            fin >> value_to_add;
            ii = 2;
        }
        for (auto i = 0; i < ii ;++i)
        {
            ++cycles;
            if (TestAndUpdateCycleToProcess(cycles, cycle_to_process))
            {
                sum_of_strngths += (cycles * X);
            }
        }
        X += value_to_add;
    }
    cout << "Part 1, sum of strengths: " << sum_of_strngths << endl;
    return 0;
}