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

string fromStringToBinaryMultiThreadedStringReserve(const string &text, vector<string> *huffmanMap, int nWorkers)
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
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " <filename> <nWorkers>" << endl;
        return 1;
    }
    string filename = argv[1];
    int nWorkers = atoi(argv[2]);

    string text = io_file::readSeq("tests/inputs/" + filename);
    vector<int> *charsFrequency = chars_frequency::computeMultiThreaded(text, nWorkers);

    huffman_tree::Node *root = huffman_tree::buildHuffmanTree(text, charsFrequency);
    vector<string> *huffmanMap = huffman_tree::buildHuffmanMap(root);
    delete charsFrequency;
    huffman_tree::deleteHuffmanTree(root);

    string binaryString1;
    {
        utimer t("Encode text (multi-threaded)");
        // Execute it 100 times to get a more precise time measurement
        for (int i = 0; i < 100; i++)
            binaryString1 = huffman_encode::fromStringToBinaryMultiThreaded(text, huffmanMap, nWorkers);
    }

    string binaryString2;
    {
        utimer t("Encode text (multi-threaded, thread merge)");
        // Execute it 100 times to get a more precise time measurement
        for (int i = 0; i < 100; i++)
            binaryString2 = fromStringToBinaryMultiThreadedStringReserve(text, huffmanMap, nWorkers);
    }

    // Verify that the two versions of the algorithm produce the same result
    if (binaryString1 != binaryString2)
    {
        cout << "ERROR: the two versions of the algorithm produce different results" << endl;
        return 1;
    }

    return 0;
}