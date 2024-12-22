#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <iterator>
#include <algorithm>

using namespace std;

bool AreAllCharactersDifferent(auto iter, const size_t &n)
{
    using value_type = typename iterator_traits<decltype(iter)>::value_type;
    auto umap = unordered_map<value_type, int>{};
    for (auto i = size_t{ 0 }; i < n ;++i,++iter)
    {
        ++umap[*iter];
    }
    return all_of(cbegin(umap), cend(umap), [](const auto &key_value){ return 1 == key_value.second; });
}

size_t GetMarkerPosition(const string &data, const size_t &n)
{
    auto pos = size(data);
    for (auto iter = cbegin(data); iter != (cend(data) - n) ;++iter)
    {
        if (AreAllCharactersDifferent(iter, n))
        {
            pos = iter - cbegin(data);
            break;
        }
    }
    return pos;
}

int main(int argc, const char *args[])
{
    auto fin = ifstream{ args[1] };
    auto data = string{};
    fin >> data;
    cout << "Part 1, first marker position: " << GetMarkerPosition(data, 4) + 4 << endl;
    cout << "Part 2, first marker position: " << GetMarkerPosition(data, 14) + 14 << endl;

    return 0;
}