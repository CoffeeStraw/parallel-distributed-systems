#include <sstream>
#include <bitset>

#include "huffman_encode.hpp"

using namespace std;

string huffman_encode::fromStringToBinarySeq(const string &text, unordered_map<char, string> &huffmanMap)
{
    string encodedText = "";
    for (auto character : text)
        encodedText += huffmanMap[character];
    return encodedText;
}

string huffman_encode::fromBinaryToASCIISeq(string binaryString)
{
    // Pad the binary string with 0s at the end, to make its length a multiple of 8
    int remainder = binaryString.length() % 8;
    if (remainder != 0)
        binaryString.append(8 - remainder, '0');

    // Convert the binary string to an ASCII string
    string asciiString = "";
    stringstream sstream(binaryString);
    while (sstream.good())
    {
        bitset<8> bits;
        sstream >> bits;
        asciiString += char(bits.to_ulong());
    }
    return asciiString;
}