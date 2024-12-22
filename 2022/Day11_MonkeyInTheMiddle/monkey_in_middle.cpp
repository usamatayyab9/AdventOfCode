#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <functional>

using namespace std;

struct Monkey
{
    vector<int>                 items;
    int                         target_monkey_true;
    int                         target_monkey_false;
    function<int(const int &)>  op;
    function<bool(const int &)> comp;

    istream& operator>>(istream &in)
    {
        auto str = string{};
        getline(in, str);
    }
};

int main()
{
    return 0;
}