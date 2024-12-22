#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <tuple>

using namespace std;

int main(int argc, const char *args[])
{
    auto vec = vector<int>{};
    for (auto [str, fin, sum] = tuple{ string{ "" }, ifstream{ args[1] }, int{ 0 } }; fin ;)
    {
        getline(fin, str);
        if (str.empty()) { vec.push_back(sum); sum = 0; }
        else             { sum += stoi(str);            }
    }
    sort(begin(vec),end(vec), std::greater{});
    for (const auto &s : vec) { cout << s << endl; }

    {
        cout << "Part 1, Elf with most calories : " << vec[0] << endl;
    }
    {
        cout << "Part 2, sum of top three calories: " << vec[0] + vec[1] + vec[2] << endl;
    }

    return 0;
}