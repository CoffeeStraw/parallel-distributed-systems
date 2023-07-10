/**
 * Test the sequential version of the Huffman encoding algorithm,
 * writing results to a csv file.
 */
#include <iostream>
#include <fstream>

#include "../src/include/io_file.hpp"
#include "../src/include/chars_frequency.hpp"
#include "../src/include/huffman_tree.hpp"
#include "../src/include/huffman_encode.hpp"
#include "../src/include/utimer.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    // Read command line arguments
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <nIterations>" << endl;
        return 1;
    }
    int nIterations = atoi(argv[1]);

    std::ofstream file("data/io.csv");
    file << "filePath,nWorkers,average" << endl;

    // Test for files with different sizes
    vector<string> fileNames = {"1MB.txt", "10MB.txt", "100MB.txt"};

    for (auto fileName : fileNames)
    {
        cout << "Testing file " << fileName << endl;

        // Compute result one time
        string text = io_file::readSeq("tests/inputs/" + fileName);

        vector<int> charsFrequency = chars_frequency::computeSeq(text, 0, text.size());

        huffman_tree::Node *root = huffman_tree::buildHuffmanTree(text, charsFrequency);
        vector<string> huffmanMap = huffman_tree::buildHuffmanMap(root);
        huffman_tree::deleteHuffmanTree(root);

        string binaryString = huffman_encode::fromStringToBinarySeq(text, 0, text.size(), huffmanMap);

        huffman_encode::padString(binaryString);
        string asciiString = huffman_encode::fromBinaryToASCIISeq(binaryString, 0, binaryString.size());

        io_file::writeSeq("tests/outputs/" + fileName, asciiString);

        // Measuring time
        long us, averageUs = 0;
        for (size_t ni = 0; ni < nIterations; ni++)
        {
            {
                utimer t("", &us);
                string text = io_file::readSeq("tests/inputs/" + fileName);
                io_file::writeSeq("tests/outputs/" + fileName, asciiString);
            }
            averageUs += us;
        }
        averageUs /= nIterations;
        file << fileName << ",1," << averageUs << endl;
        averageUs = 0;
    }

    return 0;
}