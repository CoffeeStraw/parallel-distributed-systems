#include <bitset>
#include <vector>
#include <thread>

#include "huffman_encode.hpp"

using namespace std;

string huffman_encode::fromStringToBinarySeq(const string &text, vector<string> *huffmanMap)
{
    string result = "";
    for (auto character : text)
        result += (*huffmanMap)[static_cast<unsigned char>(character)];
    return result;
}

string huffman_encode::fromStringToBinaryMultiThreaded(const string &text, vector<string> *huffmanMap, int nWorkers)
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
                    resultPartial += (*huffmanMap)[static_cast<unsigned char>(text[i])];
                chunksResult[i] = resultPartial;
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

/**
 * Pad the binary string with 0s at the end, to make its length a multiple of 8.
 */
void padString(string &binaryString)
{
    int remainder = binaryString.length() % 8;
    if (remainder != 0)
        binaryString.append(8 - remainder, '0');
}

string huffman_encode::fromBinaryToASCIISeq(string &binaryString)
{
    padString(binaryString);

    string result = "";
    for (int i = 0; i < binaryString.length(); i += 8)
    {
        bitset<8> bits(binaryString.substr(i, 8));
        result += char(bits.to_ulong());
    }

    return result;
}

string huffman_encode::fromBinaryToASCIIMultiThreaded(string &binaryString, int nWorkers)
{
    padString(binaryString);

    vector<string> chunksResult(nWorkers);

    // Static load balancing: each thread gets a chunk of the text of the same size
    vector<thread> threads;
    int chunkSize = binaryString.length() / nWorkers;
    chunkSize -= chunkSize % 8; // Make sure that the chunk size is a multiple of 8
    int start = 0;
    int end = chunkSize;

    for (int i = 0; i < nWorkers; i++)
    {
        if (i == nWorkers - 1)
            end = binaryString.length();

        threads.push_back(thread(
            [i, &binaryString, start, end, &chunksResult]()
            {
                // Do not use stringstream to avoid using substr
                string resultPartial = "";
                for (int i = start; i < end; i += 8)
                {
                    bitset<8> bits(binaryString.substr(i, 8));
                    resultPartial += char(bits.to_ulong());
                }
                chunksResult[i] = resultPartial;
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