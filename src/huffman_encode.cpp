#include <bitset>
#include <vector>
#include <thread>
#include <iostream>

#include "include/huffman_encode.hpp"

using namespace std;

string huffman_encode::fromStringToBinarySeq(const string &text, const int start, const int end, const vector<string> *huffmanMap)
{
    string result = "";
    for (int i = start; i < end; i++)
        result += (*huffmanMap)[static_cast<unsigned char>(text[i])];
    return result;
}

string huffman_encode::fromStringToBinaryMultiThreaded(const string &text, const vector<string> *huffmanMap, const int nWorkers)
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
            [&chunksResult, i, &text, start, end, &huffmanMap]()
            {
                chunksResult[i] = huffman_encode::fromStringToBinarySeq(text, start, end, huffmanMap);
            }));

        start = end;
        end += chunkSize;
    }

    // Merge
    string result = "";
    for (int i = 0; i < nWorkers; i++)
    {
        threads[i].join();
        result += chunksResult[i];
    }

    return result;
}

/**
 * Pad the binary string with 0s at the end, to make its length a multiple of 8.
 */
void huffman_encode::padString(string &binaryString)
{
    int remainder = binaryString.length() % 8;
    if (remainder != 0)
        binaryString.append(8 - remainder, '0');
}

string huffman_encode::fromBinaryToASCIISeq(string &binaryString, const int start, const int end)
{
    // We assume that the binary string has a length that is a multiple of 8
    // Remember to call padString() before calling this function
    
    string result = "";
    for (int i = start; i < end; i += 8)
    {
        bitset<8> bits(binaryString.substr(i, 8));
        result += char(bits.to_ulong());
    }

    return result;
}

string huffman_encode::fromBinaryToASCIIMultiThreaded(string &binaryString, const int nWorkers)
{
    // We assume that the binary string has a length that is a multiple of 8
    // Remember to call padString() before calling this function

    vector<string> chunksResult(nWorkers);

    // Static load balancing: each thread gets a chunk of the text of the same size
    vector<thread> threads;
    int chunkSize = binaryString.length() / nWorkers;
    chunkSize -= chunkSize % 8; // Round down to the nearest multiple of 8
    int start = 0;
    int end = chunkSize;

    for (int i = 0; i < nWorkers; i++)
    {
        if (i == nWorkers - 1)
            end = binaryString.length();

        threads.push_back(thread(
            [&chunksResult, i, &binaryString, start, end]()
            {
                chunksResult[i] = huffman_encode::fromBinaryToASCIISeq(binaryString, start, end);
            }));

        start = end;
        end += chunkSize;
    }

    string result = "";
    for (int i = 0; i < nWorkers; i++)
    {
        threads[i].join();
        result += chunksResult[i];
    }

    return result;
}