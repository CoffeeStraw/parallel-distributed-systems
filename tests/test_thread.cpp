/**
 * Test the multi-threaded version of the Huffman encoding algorithm.
 */

#include <iostream>

#include "../src/io_file.hpp"
#include "../src/chars_frequency.hpp"
#include "../src/huffman_tree.hpp"
#include "../src/huffman_encode.hpp"
#include "../src/utimer.cpp"

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

    // STEP 1: Read the input file
    string text;
    {
        utimer t("Read file (seq)");
        text = io_file::readSeq("tests/inputs/" + filename);
    }

    // STEP 2: Compute the frequency of each character
    unordered_map<char, int> *charsFrequency;
    {
        utimer t("Chars frequency (multi-threaded)");
        charsFrequency = chars_frequency::computeSeq(text, 0, text.length());
    }

    // STEP 3-4: Build the Huffman tree and the Huffman map
    unordered_map<char, string> huffmanMap;
    {
        utimer t("Huffman tree and map (seq)");
        huffman_tree::Node *root = huffman_tree::buildHuffmanTree(text, charsFrequency);
        huffman_tree::buildHuffmanMap(root, huffmanMap);
    }

    // STEP 5: Encode the text using the Huffman map
    string binaryString;
    {
        utimer t("Encode text (seq)");
        binaryString = huffman_encode::fromStringToBinaryMultiThreaded(text, huffmanMap, nWorkers);
    }

    // STEP 6: Convert the binary string to an ASCII string
    /*
    string asciiString;
    {
        utimer t("Convert to ASCII (seq)");
        asciiString = huffman_encode::fromBinaryToASCIISeq(binaryString);
    }

    // STEP 7: Write the encoded text to the output file
    {
        utimer t("Write file (seq)");
        io_file::writeSeq("tests/outputs/" + filename, asciiString);
    }
    */

    return 0;
}