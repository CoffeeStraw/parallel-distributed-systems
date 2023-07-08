/**
 * Test the multi-threaded version of the Huffman encoding algorithm.
 */

#include <iostream>

#include "../src/include/io_file.hpp"
#include "../src/include/chars_frequency.hpp"
#include "../src/include/huffman_tree.hpp"
#include "../src/include/huffman_encode.hpp"

using namespace std;

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

    // Verify that the two functions compute the same result
    string binaryString1 = huffman_encode::fromStringToBinarySeq(text, 0, text.length(), huffmanMap);
    string binaryString2 = huffman_encode::fromStringToBinaryMultiThreaded(text, huffmanMap, nWorkers);

    if (binaryString1 != binaryString2)
    {
        cout << "ERROR: the two StringToBinary compute different results" << endl;
        return 1;
    }

    // Verify that the two functions compute the same result
    huffman_encode::padString(binaryString1);
    string asciiString1 = huffman_encode::fromBinaryToASCIISeq(binaryString1, 0, binaryString1.length());
    string asciiString2 = huffman_encode::fromBinaryToASCIIMultiThreaded(binaryString1, nWorkers);

    if (asciiString1 != asciiString2)
    {
        cout << "ERROR: the two BinaryToASCII compute different results" << endl;
        return 1;
    }

    return 0;
}