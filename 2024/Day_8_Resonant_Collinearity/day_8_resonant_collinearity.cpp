
#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <locale>
#include <set>

using std::begin;
using std::cout;
using std::end;
using std::endl;
using std::getline;
using std::isalpha;
using std::isdigit;
using std::size;
using std::ifstream;
using std::pair;
using std::string;
using std::string_view;
using std::vector;
using std::unordered_map;
using std::set;

/**
 * @brief Reads the input from a file and stores it in a 2D vector of characters.
 * 
 * This function opens the file specified by the filename, reads each line of the file,
 * and stores each line as a vector of characters in a 2D vector.
 * 
 * @param filename The name of the file to read from.
 * @return A 2D vector where each inner vector represents a line from the file.
 */
vector<vector<char>> read_input(string_view filename)
{
   auto fin  = ifstream{ filename.data() };
   auto data = vector<vector<char>>{};
    for (auto line = string{}; getline(fin, line);)
    {
        data.push_back({ begin(line), end(line) });
    }
    return data;
}

bool IsValidPosition(int x, int y, int rows, int cols)
{
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

unordered_map<char, vector<pair<int, int>>> GetAntennas(const vector<vector<char>>& data)
{
    auto umap = unordered_map<char, vector<pair<int, int>>>{};
    for (auto ii = size_t{ 0 }; ii < size(data); ++ii)
    {
        for (auto jj = size_t{ 0 }; jj < size(data[ii]); ++jj)
        {
            if (const auto &kChar = data[ii][jj]; isalpha(kChar, std::locale{}) || isdigit(kChar, std::locale{}))
            {
                umap[kChar].push_back({ ii, jj });
            }
        }
    }
    return umap;
}

/**
 * @brief Solves the Day 8 puzzle, part 1.
 * 
 * This function takes a 2D vector of characters as input and returns the solution
 * to the Day 8 puzzle, part 1.
 * 
 * @param data The 2D vector of characters representing the input data.
 * @return The solution to the Day 8 puzzle, part 1.
 */

int Day8Part1(const vector<vector<char>>& data)
{
   const auto kAntennasPositionsUmap = GetAntennas(data);
   auto unique_antinodes             = set<pair<int, int>>{};
   const auto rows                   = size(data);
   const auto cols                   = size(data[0]);
    // Iterate over each frequency group
   for (const auto& [kAntenna, kPositions] : kAntennasPositionsUmap) {
        // Compare all pairs of antennas within the same frequency
        for (auto i = 0; i < size(kPositions); ++i)
        {
            for (auto j = i + 1; j < size(kPositions); ++j)
            {
                auto x1 = kPositions[i].first, y1 = kPositions[i].second;
                auto x2 = kPositions[j].first, y2 = kPositions[j].second;

                // Calculate the vector between the two antennas
                auto dx = x2 - x1;
                auto dy = y2 - y1;

                // Calculate potential antinode positions
                auto ax1 = x1 - dx, ay1 = y1 - dy; // First antinode
                auto ax2 = x2 + dx, ay2 = y2 + dy; // Second antinode

                // Add valid antinodes to the set
                if (IsValidPosition(ax1, ay1, rows, cols)) { unique_antinodes.emplace(ax1, ay1); }
                if (IsValidPosition(ax2, ay2, rows, cols)) { unique_antinodes.emplace(ax2, ay2); }
            }
        }
    }

    return size(unique_antinodes);
}

// Function to check if three points are collinear
bool areCollinear(int x1, int y1, int x2, int y2, int x3, int y3) {
    return (y2 - y1) * (x3 - x2) == (y3 - y2) * (x2 - x1);
}

int calculateAntinodesPartTwo(const vector<vector<char>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();

    const auto kAntennasPositionsUmap = GetAntennas(grid);
    // Set to store unique antinode positions
    set<pair<int, int>> uniqueAntinodes;

    // Iterate over each frequency group
    for (const auto& [frequency, positions] : kAntennasPositionsUmap) {
        int n = positions.size();

        // If there are multiple antennas, add their positions as antinodes
        if (n > 1) {
            for (const auto& [x, y] : positions) {
                uniqueAntinodes.emplace(x, y);
            }
        }

        // Compare all combinations of antennas within the same frequency
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                int x1 = positions[i].first, y1 = positions[i].second;
                int x2 = positions[j].first, y2 = positions[j].second;

                // Check all grid points to find antinodes
                for (int x = 0; x < rows; ++x) {
                    for (int y = 0; y < cols; ++y) {
                        if (areCollinear(x1, y1, x2, y2, x, y)) {
                            uniqueAntinodes.emplace(x, y);
                        }
                    }
                }
            }
        }
    }

    // Return the count of unique antinodes
    return uniqueAntinodes.size();
}


int main(int argc, char *args[])
{
    const auto data = read_input(args[1]);
    cout << "Day 8 Part 1 Solution = " << Day8Part1(data) << endl;
    cout << "Day 8 Part 2 Solution = " << calculateAntinodesPartTwo(data) << endl;
    return 0;
}