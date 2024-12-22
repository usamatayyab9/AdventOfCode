#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <stack>
#include <tuple>

using namespace std;

auto Tokenize(const string &data)
{
    auto ss  = stringstream{ data };
    auto str = string{};
    auto vec = vector<char>{};
    while (ss)
    {
        char ch1, ch2, ch3, separator;
        ss.get(ch1); ss.get(ch2); ss.get(ch3); ss.get(separator);
        // cout << "***" << ch2 << "***\n";
        vec.push_back(ch2);
        if (separator == '\n') { break; }
    }
    return vec;
}

auto ReadConfigurationFromStream(ifstream &fin)
{
    auto configuration = vector<vector<char>>{};
    for (auto str = string{}; fin ;)
    {
        getline(fin, str);
        if (str.empty()) { break; }
        // cout << str << endl;
        configuration.push_back(Tokenize(str));
        // cout << endl;
    }
    configuration.pop_back(); // remove last vector as it only contains indexing mentioned in file
    // for (const auto &row : configuration)
    // {
    //     for (const auto &col : row)
    //     {
    //         cout << col << ';';
    //     }
    //     cout << endl;
    // }
    return configuration;
}

auto ConvertConfigurationVectorToVectorStack(const vector<vector<char>> &configuration)
{
    auto vec_stack = vector<stack<char>>{};
    cout << "size of config: " << size(configuration) << endl;
    for (auto col_idx = size_t{ 0 }; col_idx < size(configuration[0]) ;++col_idx)
    {
        auto stk = stack<char>{}; // Insert in revers order in stack
        for_each(crbegin(configuration), crend(configuration), [&col_idx, &stk](const auto &row_vec)
        {
            // cout << row_vec[col_idx] << ';';
            if (row_vec[col_idx] != ' '){
                stk.push(row_vec[col_idx]);
            }
        });
        vec_stack.push_back(stk);
    }
    return vec_stack;
}

void Print(vector<stack<char>> configuration_stack)
{
    auto idx = 0;
    for (auto stk : configuration_stack)
    {
        cout << idx << ": ";
        while (false == stk.empty())
        {
            cout << stk.top() << ' ';
            stk.pop();
        }
        cout << endl;
        ++idx;
    }
}

enum class Solution
{
    kPart1,
    kPart2
};

template <Solution required_solution>
void MoveStackElemnts(stack<char> &src, stack<char> &dst, const int &n)
{
    auto vec = vector<char>{};
    for (auto i = 0; false == src.empty() && i < n ;++i)
    {
        vec.push_back(src.top());
        src.pop();
    }
    auto [first, last] = [&vec](){
        if constexpr (required_solution == Solution::kPart1) { return pair{ cbegin(vec), cend(vec)   }; }
        else                                                 { return pair{ crbegin(vec), crend(vec) }; }
    }();
    for_each(first, last, [&dst](const auto &ch)
    {
        dst.push(ch);
    });
}

template<Solution required_solution>
string ComputeSolution(vector<stack<char>> configuration_stacks, const vector<tuple<int, int, int>> &instructions_vector)
{
    for (const auto [n, from, to] : instructions_vector)
    {
        MoveStackElemnts<required_solution>(configuration_stacks[from - 1], configuration_stacks[to - 1], n);
    }
    auto str = string{};
    for_each(cbegin(configuration_stacks), cend(configuration_stacks), [&str](const auto &stk)
    {
        str.push_back(stk.top());
    });
    return str;
}

int main(int argc, const char *args[])
{
    auto fin                                = ifstream{ args[1] };
    const auto kOriginalConfigurationStacks = ConvertConfigurationVectorToVectorStack(ReadConfigurationFromStream(fin));
    auto instruction_vector                 = vector<tuple<int, int, int>>{};
    Print(kOriginalConfigurationStacks);
    for (auto [str_ignore, n, from, to] = tuple{ string{}, 0, 0, 0 };
        fin >> str_ignore >> n >> str_ignore >> from >> str_ignore >> to;
    )
    {
        instruction_vector.push_back(tuple{ n, from, to });
    }

    cout << "Part 1, top elements after operations: " << ComputeSolution<Solution::kPart1>(kOriginalConfigurationStacks, instruction_vector) << endl;
    cout << "Part 1, top elements after operations: " << ComputeSolution<Solution::kPart2>(kOriginalConfigurationStacks, instruction_vector) << endl;
    return 0;
}