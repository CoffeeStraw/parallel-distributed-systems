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

    // Csv file
    std::ofstream file("data/sequential.csv");
    file << "filePath,nWorkers,average" << endl;

    // Test for files with different sizes
    vector<string> fileNames = {"1MB.txt", "10MB.txt", "100MB.txt"};

    for (auto fileName : fileNames)
    {
        cout << "Testing file " << fileName << endl;
        long us, averageUs = 0;
        for (size_t ni = 0; ni < nIterations; ni++)
        {
            {
                utimer t("", &us);

                // STEP 1: Read the input file
                string text = io_file::readSeq("tests/inputs/" + fileName);

                // STEP 2: Compute the frequency of each character
                vector<int> charsFrequency = chars_frequency::computeSeq(text, 0, text.size());

                // STEP 3-4: Build the Huffman tree and the Huffman map
                huffman_tree::Node *root = huffman_tree::buildHuffmanTree(charsFrequency);
                vector<string> huffmanMap = huffman_tree::buildHuffmanMap(root);
                huffman_tree::deleteHuffmanTree(root);

                // STEP 5: Encode the text using the Huffman map
                string binaryString = huffman_encode::fromStringToBinarySeq(text, 0, text.size(), huffmanMap);

                // STEP 6: Convert the binary string to an ASCII string
                huffman_encode::padString(binaryString);
                string asciiString = huffman_encode::fromBinaryToASCIISeq(binaryString, 0, binaryString.size());

                // STEP 7: Write the encoded text to the output file
                io_file::writeSeq("tests/outputs/" + fileName, asciiString);
            }
            averageUs += us;
        }
        averageUs /= nIterations;
        file << fileName << ",1," << averageUs << endl;
        averageUs = 0;
    }
    cout << "Benchmark results written to data/sequential.csv" << endl;

    return 0;
}