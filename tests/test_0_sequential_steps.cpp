/**
 * Test the sequential version of the Huffman encoding algorithm,
 * writing the input+output average time to a csv file.
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
    std::ofstream file("data/io.csv");
    file << "filePath,nWorkers,average" << endl;

    // Test for files with different sizes
    vector<string> fileNames = {"1MB.txt", "10MB.txt", "100MB.txt"};
    long us, averageUs = 0;

    for (auto fileName : fileNames)
    {
        cout << "Testing file " << fileName << endl;
        long inputAverageUs, outputAverageUs = 0;

        // STEP 1: Read the input file
        string text;
        for (size_t ni = 0; ni < nIterations; ni++)
        {
            {
                utimer t("", &us);
                text = io_file::readSeq("tests/inputs/" + fileName);
            }
            averageUs += us;
        }
        averageUs /= nIterations;
        inputAverageUs = averageUs;
        cout << "Average time INPUT: " << averageUs << " us" << endl;
        averageUs = 0;

        // STEP 2: Compute the frequency of each character
        vector<int> charsFrequency;
        for (size_t ni = 0; ni < nIterations; ni++)
        {
            {
                utimer t("", &us);
                charsFrequency = chars_frequency::computeSeq(text, 0, text.size());
            }
            averageUs += us;
        }
        averageUs /= nIterations;
        cout << "Average time COUNT: " << averageUs << " us" << endl;

        // STEP 3-4: Build the Huffman tree and the Huffman map
        vector<string> huffmanMap;
        for (size_t ni = 0; ni < nIterations; ni++)
        {
            {
                utimer t("", &us);
                huffman_tree::Node *root = huffman_tree::buildHuffmanTree(charsFrequency);
                huffmanMap = huffman_tree::buildHuffmanMap(root);
                huffman_tree::deleteHuffmanTree(root);
            }
            averageUs += us;
        }
        averageUs /= nIterations;
        cout << "Average time TREE+MAP: " << averageUs << " us" << endl;

        // STEP 5: Encode the text using the Huffman map
        string binaryString;
        for (size_t ni = 0; ni < nIterations; ni++)
        {
            {
                utimer t("", &us);
                binaryString = huffman_encode::fromStringToBinarySeq(text, 0, text.size(), huffmanMap);
            }
            averageUs += us;
        }
        averageUs /= nIterations;
        cout << "Average time ENCODE: " << averageUs << " us" << endl;

        // STEP 6: Convert the binary string to an ASCII string
        string asciiString;
        for (size_t ni = 0; ni < nIterations; ni++)
        {
            {
                utimer t("", &us);
                huffman_encode::padString(binaryString);
                asciiString = huffman_encode::fromBinaryToASCIISeq(binaryString, 0, binaryString.size());
            }
            averageUs += us;
        }
        averageUs /= nIterations;
        cout << "Average time ASCII: " << averageUs << " us" << endl;

        // STEP 7: Write the encoded text to the output file
        for (size_t ni = 0; ni < nIterations; ni++)
        {
            {
                utimer t("", &us);
                io_file::writeSeq("tests/outputs/" + fileName, asciiString);
            }
            averageUs += us;
        }
        averageUs /= nIterations;
        outputAverageUs = averageUs;
        cout << "Average time OUTPUT: " << averageUs << " us" << endl;

        // Write results to csv file
        file << fileName << ",1," << inputAverageUs + outputAverageUs << endl;
    }
    return 0;
}