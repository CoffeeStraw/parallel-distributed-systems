/**
 * Test the multi-threaded version of the Huffman encoding algorithm.
 */

#include <iostream>
#include <thread>
#include <mutex>

#include "../src/include/io_file.hpp"
#include "../src/include/chars_frequency.hpp"
#include "../src/include/huffman_tree.hpp"
#include "../src/include/huffman_encode.hpp"
#include "../src/include/utimer.hpp"

using namespace std;

mutex mtx;

string fromStringToBinaryMultiThreadedStringReserve(const string &text, vector<string> &huffmanMap, int nWorkers)
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
                    resultPartial += huffmanMap[static_cast<unsigned char>(text[i])];
                chunksResult[i] = resultPartial;
            }));

        start = end;
        end += chunkSize;
    }

    string result = "";
    result.reserve(text.length());
    for (int i = 0; i < nWorkers; i++)
    {
        threads[i].join();
        result += chunksResult[i];
    }

    return result;
}

int main(int argc, char *argv[])
{
    // Read command line arguments
    if (argc != 4)
    {
        cout << "Usage: " << argv[0] << " <filename> <nWorkers> <nIterations)" << endl;
        return 1;
    }
    string filename = argv[1];
    int nWorkers = atoi(argv[2]);
    int nIterations = atoi(argv[3]);

    string text = io_file::readSeq("tests/inputs/" + filename);
    vector<int> charsFrequency = chars_frequency::computeSeq(text, 0, text.length());
    huffman_tree::Node *root = huffman_tree::buildHuffmanTree(text, charsFrequency);
    vector<string> huffmanMap = huffman_tree::buildHuffmanMap(root);
    huffman_tree::deleteHuffmanTree(root);

    string binaryString;

    // Seqeuential execution
    long us, averageUs = 0;
    for (int i = 0; i < nIterations; i++)
    {
        {
            utimer t("", &us);
            binaryString = huffman_encode::fromStringToBinarySeq(text, 0, text.length(), huffmanMap);
        }
        averageUs += us;
    }
    averageUs /= nIterations;
    cout << "Average sequential execution time: " << averageUs << " us" << endl;

    // Multi-threaded execution
    us, averageUs = 0;
    for (int i = 0; i < nIterations; i++)
    {
        {
            utimer t("", &us);
            binaryString = fromStringToBinaryMultiThreadedStringReserve(text, huffmanMap, nWorkers);
        }
        averageUs += us;
    }
    averageUs /= nIterations;
    cout << "Average multi-threaded execution time: " << averageUs << " us" << endl;

    // FastFlow execution
    us, averageUs = 0;
    for (int i = 0; i < nIterations; i++)
    {
        {
            utimer t("", &us);
            binaryString = huffman_encode::fromStringToBinaryFastFlow(text, huffmanMap, nWorkers);
        }
        averageUs += us;
    }
    averageUs /= nIterations;
    cout << "Average FastFlow execution time: " << averageUs << " us" << endl;

    return 0;
}