/**
 * @file packet_decoder.cpp
 * @author Usama Uayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 ./packet_decoder.cpp
 * Compiled with gcc version 9.4.0
 * Problem statement : https://adventofcode.com/2021/day/16
 * 
 * Solution:
 * For implementing this solution follwing are provided:
 * - A function `ReadInput()` for reading the input file.
 * 
 * - A struct `PacketStructure` which stores info about a packet and sub-packets.
 * 
 * - A struct `PacketParser` for parsing hex string into packet structure.
 * 
 * - A function `SumOfVersionNumbers` which recursivley calculates the sum of version numbers
 * of all packets.
 * 
 * - A function `EvalulatePacket()` which recursively computes all packet operations based on their type
 * 
 * Driver code:
 * - The program expects one additional argument which should be the input filename.
 * - Prints part 1 solution i.e. sum of version numbers of all packets
 * - Prints part 2 solution i.e. expression result of all packets.
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <iterator>
#include <iomanip>
#include <unordered_map>
#include <string_view>
#include <numeric>
#include <optional>
#include <charconv>
#include <stack>

using namespace std;

/**
 * @brief Opens the file, reads and returns the string
 * @param filename 
 * @return string - Contained in file
 */
string ReadInput(const string &filename)
{
    auto fin = ifstream{filename};
    return {istream_iterator<char>{fin}, istream_iterator<char>{}};
}

/**
 * @brief A recrusive structure which stores info about a packet and its sub-packets
 * 
 */
struct PacketStructure
{
    long long int version;
    long long int typeID;
    optional<long long int> literal;
    vector<PacketStructure> sub_packets;
};

/**
 * @brief Prints packet info on stream object
 * @tparam StreamType 
 * @param out 
 * @param packet_data 
 * @return 
 */
template<typename StreamType>
StreamType& operator<<(StreamType &out, const PacketStructure &packet_data)
{
    out << "Version: " << packet_data.version << ", ";
    out << "TypeID : " << packet_data.typeID  << ", ";
    
    if (packet_data.literal.has_value()) { out << "Literal: " << packet_data.literal.value(); }
    else                                 { out << "Literal: " << "N/A";                       }
    cout << "\n";
    for (const auto &kSubPacket : packet_data.sub_packets)
    {
        out << kSubPacket;
    }
    return out;
}

/**
 * @brief Structure for parsing packet string
 * Function operator overload is provided should be called
 * for parsing hex string
 * 
 */
struct PacketParser
{
    static constexpr auto kVersionBitSize = 3;
    static constexpr auto kTypeIDBitSize  = 3;
    static constexpr auto kLiteralTypeId  = 4;

    /**
     * @brief Converts the hex string to binary string and parses the binary 
     * string into PacketStructure
     * @param hex_data - string which contains hex data 
     * @return Parsed Packet
     */
    PacketStructure operator()(const string &hex_data)
    {
        const auto kBinaryData         = ConvertHexStringToBitString(hex_data);
        auto [packets, bits_processed] = ExtractPacket_REC(kBinaryData);
        return packets;
    }

    /**
     * @brief Replaces each character in the string param hex_data with its binary
     * equivalent. Binary of each character is confined to 4 bits.
     * @param hex_data 
     * @return Binar representation on hex string
     */
    string ConvertHexStringToBitString(const string &hex_data)
    {
        static const auto kHexToBinaryUmap = std::unordered_map<char, string_view>
        {
            {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"},
            {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"},
            {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"},
            {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"}
        };
        auto binary_data = string{};
        for (const auto &kCh : hex_data)
        {
            binary_data.append(kHexToBinaryUmap.at(kCh));
        }
        return binary_data;
    }

    /**
     * @brief Parses the bit string as per the rules and returns the Packet structure
     * which points to the root packet.
     * @param binary_view 
     * @return pair<PacketStructure, size_t> 
     */
    pair<PacketStructure, size_t> ExtractPacket_REC(string_view binary_view)
    {
        const auto kVersion    = BinaryToDecimal(binary_view.substr(0, kVersionBitSize));//First 3 bits are version bits
        const auto kTypeID     = BinaryToDecimal(binary_view.substr(3, kTypeIDBitSize));//Next 3 bits are representing typeID
        auto packets_structure = PacketStructure{kVersion, kTypeID};
        auto bits_processed    = size_t{ kVersionBitSize + kTypeIDBitSize };
        binary_view.remove_prefix(bits_processed);
        if (kLiteralTypeId == kTypeID)
        {
            const auto kLiteralsGroups  = FetchLiteralGroups(binary_view);
            packets_structure.literal   = ConvertLiteralGroups(kLiteralsGroups);
            bits_processed             += accumulate(cbegin(kLiteralsGroups), cend(kLiteralsGroups), size_t{ 0 },
            [](const auto init, const auto literal_str){
                return init + size(literal_str);
            });
        }
        else
        {
            const auto kLengthTypeID = binary_view.front() - '0';
            bits_processed          += 1;
            binary_view.remove_prefix(1);
            if (0 == kLengthTypeID)
            {
                /**
                 * If the length type ID is 0, then the next 15 bits are a number that represents
                 * the total length in bits of the sub-packets contained by this packet
                 */
                const auto kLengthOfSubPacketsInBits = BinaryToDecimal(binary_view.substr(0, 15));
                bits_processed                      += 15;
                binary_view.remove_prefix(15);
                for (auto subpacket_bits = size_t{ 0 }; subpacket_bits < kLengthOfSubPacketsInBits ;)
                {
                    const auto [kSubPackets, kSubPacketsBitsProcessed] = ExtractPacket_REC(binary_view);
                    subpacket_bits += kSubPacketsBitsProcessed;
                    bits_processed += kSubPacketsBitsProcessed;
                    packets_structure.sub_packets.push_back(kSubPackets);
                    binary_view.remove_prefix(kSubPacketsBitsProcessed);
                }
            }
            else if (1 == kLengthTypeID)
            {
                /**If the length type ID is 1, then the next 11 bits are a number that represents
                 * the number of sub-packets immediately contained by this packet.
                */
                const auto kNumberOfSubPackets = BinaryToDecimal(binary_view.substr(0, 11));
                bits_processed                += 11;
                binary_view.remove_prefix(11);
                for (auto sub_packets_extracted = size_t{ 0 }; sub_packets_extracted < kNumberOfSubPackets ;++sub_packets_extracted)
                {
                    const auto [kSubPackets, kSubPacketsBitsProcessed] = ExtractPacket_REC(binary_view);
                    bits_processed          += kSubPacketsBitsProcessed;
                    packets_structure.sub_packets.push_back(kSubPackets);
                    binary_view.remove_prefix(kSubPacketsBitsProcessed);
                }
            }
        }
        return { packets_structure, bits_processed };
    }

    /**
     * @brief Converts binary to decimal
     * 
     * @param data_view 
     * @return long long int 
     */
    long long int BinaryToDecimal(string_view data_view)
    {
        long long int value{};
        from_chars(data_view.cbegin(), data_view.cend(), value, 2);
        return value;
    }

    /**
     * @brief 
     * Packets with type ID 4 represent a literal value. Literal value packets encode
     * a single binary number. To do this, the binary number is padded with leading zeroes
     * until its length is a multiple of four bits, and then it is broken into groups of
     * four bits. Each group is prefixed by a 1 bit except the last group, which is prefixed
     * by a 0 bit. These groups of five bits immediately follow the packet header
     * @param binary_data 
     * @return vector of literals such that padded character is part of each literals
     * 
     * @note padded character from each literal is not removed, it is caller's duty
     * to remove the padded character before processing
     */
    vector<string_view> FetchLiteralGroups(string_view binary_data)
    {
        auto literal_groups = vector<string_view>{};
        auto first_value    = '\0';
        do {
            first_value = binary_data.front();
            literal_groups.push_back(string_view{ binary_data.data(), 5 });
            binary_data.remove_prefix(5); //length of each group is 5
        } while (first_value != '0');
        return literal_groups;
    }

    /**
     * @brief All the literals in the list param literal_groups are first flatten
     * into a single string with padded character removed. Then converts string
     * into decimal
     * @param literal_groups 
     * @return 
     */
    long long int ConvertLiteralGroups(const vector<string_view> &literal_groups)
    {
        auto literal_str = string{};
        for (const auto &kLiteralSubGroup : literal_groups)
        {
            /**Each group is prefixed by a 1 bit except the last group, which is prefixed by a 0 bit. So therefore
             * ignore first bit.
            */
            literal_str.append(cbegin(kLiteralSubGroup) + 1, cend(kLiteralSubGroup));
        }
        return BinaryToDecimal(literal_str);
    }
};

/**
 * @brief Returns the sum of all version numbers of all packets.
 * Recursively iterates all packets and accumulates their version numbers
 * sum
 * @param packets - Root packet
 * @return size_t - sum of all version numbers of all packets
 */
size_t SumOfVersionNumbers(const PacketStructure &packets)
{
    return accumulate(cbegin(packets.sub_packets), cend(packets.sub_packets), packets.version,
        [](auto init, const auto &packet_info){ return init + SumOfVersionNumbers(packet_info); });
}

/**
 * @brief Evaluates the packet tree structure and returns the final value
 * @param packets 
 * @return 
 */
size_t EvalulatePacket(const PacketStructure &packets)
{
    constexpr auto PacketComparator = [](const PacketStructure &p1, const PacketStructure &p2){
        return EvalulatePacket(p1) < EvalulatePacket(p2);
    };
    auto return_value = size_t{};
    switch (packets.typeID)
    {
        case 4 : { return_value = packets.literal.value(); break; }
        case 0 : {
            /**
             * Packets with type ID 0 are sum packets - their value is the sum of the values of their sub-packets.
             * If they only have a single sub-packet, their value is the value of the sub-packet.
            */
            return_value = accumulate(cbegin(packets.sub_packets), cend(packets.sub_packets), size_t{ 0 }, 
            [](const auto init, const PacketStructure &sub_packet){
                return init + EvalulatePacket(sub_packet);
            });
            break;
        }
        case 1 : {
            /**Packets with type ID 1 are product packets - their value is the result of multiplying together 
             * the values of their sub-packets. If they only have a single sub-packet, their value is the value
             * of the sub-packet.
             **/
            return_value = accumulate(cbegin(packets.sub_packets), cend(packets.sub_packets), size_t{ 1 }, 
            [](const auto init, const PacketStructure &sub_packet){
                return init * EvalulatePacket(sub_packet);
            });
            break;
        }
        case 2 : { 
            /*!Packets with type ID 2 are minimum packets - their value is the minimum of the values of their sub-packets.*/
            return_value = EvalulatePacket(*min_element(cbegin(packets.sub_packets), cend(packets.sub_packets), PacketComparator));
            break;
        }
        case 3 : {
            /*!Packets with type ID 3 are maximum packets - their value is the maximum of the values of their sub-packets.*/
            return_value = EvalulatePacket(*max_element(cbegin(packets.sub_packets), cend(packets.sub_packets), PacketComparator));
            break;
        }
        case 5 : {
            /*!Packets with type ID 5 are greater than packets - their value is 1 if the value of the
            first sub-packet is greater than the value of the second sub-packet; otherwise, their
            value is 0. These packets always have exactly two sub-packets.*/
            return_value = EvalulatePacket(packets.sub_packets[0]) > EvalulatePacket(packets.sub_packets[1]);
            break;
        }
        case 6 : {
            /*!Packets with type ID 6 are less than packets - their value is 1 if the value of the
            first sub-packet is less than the value of the second sub-packet; otherwise, their
            value is 0. These packets always have exactly two sub-packets.*/
            return_value = EvalulatePacket(packets.sub_packets[0]) < EvalulatePacket(packets.sub_packets[1]);
            break;
        }
        case 7 : {
            /*!Packets with type ID 7 are equal to packets - their value is 1 if the value of the
            first sub-packet is equal to the value of the second sub-packet; otherwise, their
            value is 0. These packets always have exactly two sub-packets.*/
            return_value = EvalulatePacket(packets.sub_packets[0]) == EvalulatePacket(packets.sub_packets[1]);
            break;
        }

        default : { break; }
    }
    return return_value;
}

int main(int argc, const char *args[])
{
    auto data    = ReadInput(args[1]);
    auto packets = PacketParser{}(data);

    cout << "Part 1(Sum of version numbers): " << SumOfVersionNumbers(packets) << endl;
    cout << "Part 2(Expression value)      : " << EvalulatePacket(packets) << endl;
    return 0;
}