/**
 * @file day_9_disk_fragmenter.cpp
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief
 * Compile command : g++ -std=c++17 .\day_9_disk_fragmenter.cpp
 * g++ version 13.2.0
 * @version 0.1
 * @date 2025-01-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <iostream>
#include <type_traits>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <fstream>
#include <string_view>
#include <numeric>
#include <memory>
#include <variant>

using std::accumulate;
using std::cout;
using std::endl;
using std::get;
using std::holds_alternative;
using std::ifstream;
using std::pair;
using std::string;
using std::string_view;
using std::vector;
using std::variant;


struct EmptySpace {
    size_t blocks{ 0 };
};

struct FileBlock {
    size_t id;
    size_t blocks{ 0 };
};

using DiskBlock = variant<EmptySpace, FileBlock>;

void PrintDiskBlock(const DiskBlock &block)
{
    if (holds_alternative<EmptySpace>(block))
    {
        cout << ".";
    }
    else
    {
        cout << get<FileBlock>(block).id;
    }
}

void PrintDiskMap(const vector<DiskBlock> &disk_map)
{
    for (const auto &block : disk_map)
    {
        PrintDiskBlock(block);
    }
    cout << endl;
}


string ReadInput(string_view filename)
{
    auto file  = ifstream { filename.data() };
    auto input = string{};
    file >> input;
    return input;
}

/**
 * @brief Calculates checksum of the disk map by mupltiplying the index of the block with the file id then summing them up.
 * @param disk_map 
 * @return size_t 
 */
size_t CalculateChecksum(const vector<DiskBlock> &disk_map)
{
    return accumulate(begin(disk_map), end(disk_map), size_t{ 0 }, [index = 0](auto sum, auto &block) mutable { 
        if (holds_alternative<FileBlock>(block))
        {
            sum += index * get<FileBlock>(block).id;
        }
        ++index;
        return sum;
    });
}

/**
 * @brief Converts the input string to disk map. 
 * @param input 
 * @return vector<DiskBlock> 
 */
vector<DiskBlock> ConvertStringToDiskMap(const string &input)
{
    auto disk_map       = vector<DiskBlock>{};
    for_each(begin(input), end(input), [&disk_map, is_empty_space = false, file_id = size_t{ 0 }](const auto &kCh) mutable
    {
        const auto kCount = kCh - '0';
        disk_map.insert(end(disk_map), kCount, is_empty_space ? DiskBlock{ EmptySpace{} } : DiskBlock{ FileBlock{ file_id } });
        file_id       += is_empty_space;
        is_empty_space = !is_empty_space;
    });
    return disk_map;
}

/**
 * @brief Partitions the disk map into two parts based on the predicate.
 * @param first 
 * @param last 
 * @param pred 
 * @return auto 
 */
size_t Day9Part1(const string& input)
{
    auto disk_map = ConvertStringToDiskMap(input);
    //Shifts the empty spaces to the end of the disk map.
    partition(begin(disk_map), end(disk_map), [](auto &block) { return holds_alternative<FileBlock>(block); }); 
    const auto kCheckSum = CalculateChecksum(disk_map);
    return kCheckSum;
}

pair<int, int> ReverseFindFileBlock(const vector<DiskBlock> &disk_map, const int &index)
{
    // auto first = make_reverse_iterator(cbegin(disk_map) + index);
    // auto last  = crend(disk_map);
    // auto file_block_start_pos = find_if(first, last, [](const auto &block) { return holds_alternative<FileBlock>(block); });
    // auto file_block_end_pos   = find_if_not(file_block_start_pos, last, [kFileId = get<FileBlock>(*file_block_start_pos).id](const auto &block)
    // {
    //     return holds_alternative<FileBlock>(block) && get<FileBlock>(block).id == kFileId;
    // });
    auto idx = index;
    for (; idx >= 0 && holds_alternative<EmptySpace>(disk_map[idx]) ; --idx) {}
    auto end_idx = idx;
    for (const auto kFileId = get<FileBlock>(disk_map[idx]).id; 
        idx >= 0 &&
        holds_alternative<FileBlock>(disk_map[idx]) && 
        get<FileBlock>(disk_map[idx]).id == kFileId; 
        --idx)
    {}
    return { idx + 1, end_idx - idx };
}

pair<int, int> FindEmptyBlock(const vector<DiskBlock> &disk_map, const int &first_idx, const int &last_idx)
{
    auto EmptyBlockPredicate   = [](const auto &block) { return holds_alternative<EmptySpace>(block); };
    auto empty_block_start_pos = find_if    (cbegin(disk_map) + first_idx, cbegin(disk_map) + last_idx, EmptyBlockPredicate);
    auto empty_block_end_pos   = find_if_not(empty_block_start_pos       , cbegin(disk_map) + last_idx, EmptyBlockPredicate);
    
    return { empty_block_start_pos - cbegin(disk_map), empty_block_end_pos - empty_block_start_pos };
}

pair<int, int> FindEmptyBlockIfSize(const vector<DiskBlock> &disk_map, const int &first_idx, const int &last_idx, const int &size)
{
    auto idx                    = first_idx;
    auto replacement_block_info = pair<int, int>{ -1, 0 };
    while (idx < last_idx)
    {
        const auto [kStartIdx, kLength] = FindEmptyBlock(disk_map, idx, last_idx);
        if (kLength >= size)
        {
            replacement_block_info = { kStartIdx, kLength };
            break;
        }
        idx = kStartIdx + kLength;
    }
    return replacement_block_info;
}

size_t Day9Part2(const string& input)
{
    auto disk_map = ConvertStringToDiskMap(input);

    for (auto idx = static_cast<int>(size(disk_map) - 1); idx >= 0;)
    {
        const auto [kStartIdx, kFileBlockCount] = ReverseFindFileBlock(disk_map, idx);
        if (kFileBlockCount >= 1)
        {
            const auto [kEmptyBlockStartIdx, kEmptyBlockCount] = FindEmptyBlockIfSize(disk_map, 0, kStartIdx, kFileBlockCount);
            if (kEmptyBlockCount >= kFileBlockCount)
            {
                swap_ranges(begin(disk_map) + kStartIdx, begin(disk_map) + kStartIdx + kFileBlockCount, begin(disk_map) + kEmptyBlockStartIdx);
            }
        }
        idx = kStartIdx - 1;
    }
    
    const auto kCheckSum = CalculateChecksum(disk_map);
    return kCheckSum;
}

int main(const int argc, const char* argv[])
{
    auto input = ReadInput(argv[1]);
    cout << "Day 9, Part 1: " << Day9Part1(input) << endl;
    cout << "Day 9, Part 2: " << Day9Part2(input) << endl;
    return 0;
}
