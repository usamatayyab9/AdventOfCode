#include <iostream>
#include <string>
#include <fstream>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>
#include <numeric>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>

using std::accumulate;
using std::cbegin;
using std::cend;
using std::cout;
using std::ifstream;
using std::map;
using std::minmax_element;
using std::pair;
using std::string_view;
using std::string;
using std::unordered_map;
using std::vector;

auto ReadInput(string_view filename)
{
    auto fin          = ifstream{ filename.data() };
    auto template_str = string{};
    auto rules_map    = map<pair<char, char>, char>{};
    fin >> template_str;
    fin >> std::ws;
    
    auto str        = string{};
    while (getline(fin, str))
    {
        //say rule is AB -> C
        const auto kPairing    = pair{ str[0], str[1] }; //extract first two charcters i.e. AB from above string
        const auto kMappedChar = str.back(); //extract last character i.e. C from above string
        rules_map[kPairing]    = kMappedChar;
    }
    return pair{ template_str, rules_map };
}

size_t ApplyPolymerInsertionRules(string_view template_str, const map<pair<char, char>, char> &kRulesUmap, const int &n)
{
    //Generate character count inital template
    auto character_count_umap = unordered_map<char, size_t>{};
    for (const auto &ch : template_str)
    { 
        ++character_count_umap[ch];
    }
    //Insert all adjacent pairs from template string into map
    auto pairs_count_map = map<pair<char, char>, size_t>{};
    for (auto idx = 0; idx < size(template_str) - 1 ;++idx)
    {
        ++pairs_count_map[{template_str[idx], template_str[idx + 1]}];
    }

    for (auto ii = 0; ii < n ;++ii)
    {
        auto new_pairs_count_map = map<pair<char, char>, size_t>{};//For storing new pairs that are generated
        for (auto &[current_pair, count] : pairs_count_map)
        {
            if (count > 0)
            {
                const auto kMappedChar                   = kRulesUmap.at(current_pair);
                character_count_umap[kMappedChar]       += count;
                const auto kLeftPair                     = pair{ current_pair.first, kMappedChar  };
                const auto kRightPair                    = pair{ kMappedChar, current_pair.second };
                new_pairs_count_map[kLeftPair]          += count;
                new_pairs_count_map[kRightPair]         += count;
                count                                    = 0;
            }
        }
        for (const auto &[kNewInsertedPair, kNewPairCount] : new_pairs_count_map)
        {
            pairs_count_map[kNewInsertedPair] += kNewPairCount;
        }
    }

    auto [min_pos, max_pos] = minmax_element(cbegin(character_count_umap), cend(character_count_umap), [](const auto &p1, const auto &p2){
        return p1.second < p2.second;
    });
    const auto &[kMaxOcurringChar, kMaxCount] = *max_pos;
    const auto &[kMinOcurringChar, kMinCount] = *min_pos;
    return kMaxCount - kMinCount;
}

int main(int argc, const char *args[])
{
    cout << "Taking input from file : " << args[1] << "\n";
    const auto [kTemplatesStr, kRulesMap] = ReadInput(args[1]);
    cout << "Day 14 : Extended Polymerization:\n";
    cout << "Part 1 (10 steps): " << ApplyPolymerInsertionRules(kTemplatesStr, kRulesMap, 10) << "\n";
    cout << "Part 2 (40 steps): " << ApplyPolymerInsertionRules(kTemplatesStr, kRulesMap, 40) << "\n";
    return 0;
}