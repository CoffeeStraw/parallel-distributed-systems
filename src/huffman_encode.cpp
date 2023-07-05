#include <sstream>
#include <bitset>
#include <vector>
#include <thread>

#include "huffman_encode.hpp"

using namespace std;

string huffman_encode::fromStringToBinarySeq(const string &text, unordered_map<char, string> &huffmanMap)
{
    string result = "";
    for (auto character : text)
        result += huffmanMap[character];
    return result;
}

string huffman_encode::fromStringToBinaryMultiThreaded(const string &text, unordered_map<char, string> &huffmanMap, int nWorkers)
{
    vector<string> chunksResult(nWorkers);

    // Static load balancing: each thread gets a chunk of the text of the same size
    vector<thread> threads;
    int chunkSize = text.length() / nWorkers;
    int start = 0;
    int end = chunkSize;

    for (int i = 0; i < nWorkers; i++)
    {
        if (i == nWorkers - 1)
            end = text.length();

        threads.push_back(thread(
            [i, &text, start, end, &huffmanMap, &chunksResult]()
            {
                string resultPartial = "";
                for (int i = start; i < end; i++)
                    resultPartial += huffmanMap[text[i]];
                chunksResult[i] = resultPartial;
            }));
        
        start = end;
        end += chunkSize;
    }

    for (auto &thread : threads)
        thread.join();

    string result = "";
    for (auto &partialString : chunksResult)
        result += partialString;
    return result;
}

string huffman_encode::fromBinaryToASCIISeq(string binaryString)
{
    // Pad the binary string with 0s at the end, to make its length a multiple of 8
    int remainder = binaryString.length() % 8;
    if (remainder != 0)
        binaryString.append(8 - remainder, '0');

    // Convert the binary string to an ASCII string
    string result = "";
    stringstream sstream(binaryString);
    while (sstream.good())
    {
        bitset<8> bits;
        sstream >> bits;
        result += char(bits.to_ulong());
    }
    return result;
}