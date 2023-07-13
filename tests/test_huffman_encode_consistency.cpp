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
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <nWorkers>" << endl;
        return 1;
    }
    string filename = "100MB.txt";
    int nWorkers = atoi(argv[1]);

    // Run required steps to reach the COUNT stage
    string text = io_file::readSeq("tests/inputs/" + filename);
    vector<int> charsFrequency = chars_frequency::computeMultiThreaded(text, nWorkers);
    huffman_tree::Node *root = huffman_tree::buildHuffmanTree(charsFrequency);
    vector<string> huffmanMap = huffman_tree::buildHuffmanMap(root);

    // ENCODE stage: check for consistency between the 3 versions
    string binaryString1 = huffman_encode::fromStringToBinarySeq(text, 0, text.length(), huffmanMap);
    string binaryString2 = huffman_encode::fromStringToBinaryMultiThreaded(text, huffmanMap, nWorkers);
    string binaryString3 = huffman_encode::fromStringToBinaryFastFlow(text, huffmanMap, nWorkers);

    if (binaryString1 != binaryString2 || binaryString1 != binaryString3)
    {
        cout << "ERROR: the three StringToBinary compute different results" << endl;
        return 1;
    }

    // ASCII stage: check for consistency between the 3 versions
    huffman_encode::padString(binaryString1);
    string asciiString1 = huffman_encode::fromBinaryToASCIISeq(binaryString1, 0, binaryString1.length());
    string asciiString2 = huffman_encode::fromBinaryToASCIIMultiThreaded(binaryString1, nWorkers);
    string asciiString3 = huffman_encode::fromBinaryToASCIIFastFlow(binaryString1, nWorkers);

    if (asciiString1 != asciiString2 || asciiString1 != asciiString3)
    {
        cout << "ERROR: the three BinaryToASCII compute different results" << endl;
        return 1;
    }

    cout << "Test passed: the three versions compute the same result" << endl;

    return 0;
}