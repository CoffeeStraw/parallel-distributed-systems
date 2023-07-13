/**
 * Test the multi-threaded version of the Huffman encoding algorithm.
 */

#include <iostream>
#include <thread>

#include "../src/include/io_file.hpp"
#include "../src/include/chars_frequency.hpp"
#include "../src/include/huffman_tree.hpp"
#include "../src/include/huffman_encode.hpp"
#include "../src/include/utimer.hpp"

using namespace std;

string fromStringToBinaryMultiThreadedMeasureOverhead(const string &text, const vector<string> &huffmanMap, const int nWorkers, long &totalUs)
{
    // Compute in advance chunksResult, as it is not overhead
    vector<string> chunksResult(nWorkers);
    int chunkSize = text.length() / nWorkers;
    int start = 0;
    int end = chunkSize;

    for (int i = 0; i < nWorkers; i++)
    {
        if (i == nWorkers - 1)
            end = text.length();

        chunksResult[i] = huffman_encode::fromStringToBinarySeq(text, start, end, huffmanMap);

        start = end;
        end += chunkSize;
    }

    // Overhead computation
    string result = "";
    long us;

    // Compute overhead for creating threads
    vector<thread> threads;
    us = 0;
    {
        utimer t("Overhead threads", &us);

        chunkSize = text.length() / nWorkers;
        start = 0;
        end = chunkSize;

        for (int i = 0; i < nWorkers; i++)
        {
            if (i == nWorkers - 1)
                end = text.length();

            threads.push_back(thread([&]() {}));
            start = end;
            end += chunkSize;
        }
    }
    totalUs += us;

    // Measure overhead for merging
    us = 0;
    {
        utimer t("Overhead merging", &us);
        // Merge
        for (int i = 0; i < nWorkers; i++)
        {
            threads[i].join();
            result += chunksResult[i];
        }
    }
    totalUs += us;

    return result;
}

string fromBinaryToASCIIMultiThreadedMeasureOverhead(const string &text, const int nWorkers, long &totalUs)
{
    // Compute in advance chunksResult, as it is not overhead
    vector<string> chunksResult(nWorkers);
    int chunkSize = text.length() / nWorkers;
    int start = 0;
    int end = chunkSize;

    for (int i = 0; i < nWorkers; i++)
    {
        if (i == nWorkers - 1)
            end = text.length();

        chunksResult[i] = huffman_encode::fromBinaryToASCIISeq(text, start, end);

        start = end;
        end += chunkSize;
    }

    // Overhead computation
    string result = "";
    long us;

    // Compute overhead for creating threads
    vector<thread> threads;
    us = 0;
    {
        utimer t("Overhead threads", &us);

        chunkSize = text.length() / nWorkers;
        start = 0;
        end = chunkSize;

        for (int i = 0; i < nWorkers; i++)
        {
            if (i == nWorkers - 1)
                end = text.length();

            threads.push_back(thread([&]() {}));
            start = end;
            end += chunkSize;
        }
    }
    totalUs += us;

    // Measure overhead for merging
    us = 0;
    {
        utimer t("Overhead merging", &us);
        // Merge
        for (int i = 0; i < nWorkers; i++)
        {
            threads[i].join();
            result += chunksResult[i];
        }
    }
    totalUs += us;

    return result;
}

int main(int argc, char *argv[])
{
    // Read command line arguments
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " <nWorkers> <nIterations>" << endl;
        return 1;
    }
    string filename = "100MB.txt";
    int nWorkers = atoi(argv[1]);
    int nIterations = atoi(argv[2]);

    // Run required steps to reach the COUNT stage
    string text = io_file::readSeq("tests/inputs/" + filename);
    vector<int> charsFrequency = chars_frequency::computeMultiThreaded(text, nWorkers);
    huffman_tree::Node *root = huffman_tree::buildHuffmanTree(charsFrequency);
    vector<string> huffmanMap = huffman_tree::buildHuffmanMap(root);
    string binaryString;

    // Multi-threaded execution
    long us, averageUs = 0;
    for (int i = 0; i < nIterations; i++)
    {
        {
            utimer t("", &us);
            binaryString = huffman_encode::fromStringToBinaryMultiThreaded(text, huffmanMap, nWorkers);
        }
        averageUs += us;
    }
    averageUs /= nIterations;
    cout << "Average StringToBinary multi-threaded execution time: " << averageUs << " us" << endl;

    // Multi-threaded execution (measure overhead)
    us, averageUs = 0;
    for (int i = 0; i < nIterations; i++)
    {
        us = 0;
        binaryString = fromStringToBinaryMultiThreadedMeasureOverhead(text, huffmanMap, nWorkers, us);
        averageUs += us;
    }
    averageUs /= nIterations;
    cout << "Average StringToBinary multi-threaded execution time (measure overhead): " << averageUs << " us" << endl;

    // Pad the binary string one time only (not an overhead)
    huffman_encode::padString(binaryString);
    string asciiString;

    // Multi-threaded execution
    us, averageUs = 0;
    for (int i = 0; i < nIterations; i++)
    {
        {
            utimer t("", &us);
            asciiString = huffman_encode::fromBinaryToASCIIMultiThreaded(binaryString, nWorkers);
        }
        averageUs += us;
    }
    averageUs /= nIterations;
    cout << "Average BinaryToASCII multi-threaded execution time: " << averageUs << " us" << endl;

    // Multi-threaded execution (measure overhead)
    us, averageUs = 0;
    for (int i = 0; i < nIterations; i++)
    {
        us = 0;
        asciiString = fromBinaryToASCIIMultiThreadedMeasureOverhead(binaryString, nWorkers, us);
        averageUs += us;
    }
    averageUs /= nIterations;
    cout << "Average BinaryToASCII multi-threaded execution time (measure overhead): " << averageUs << " us" << endl;

    return 0;
}