/**
 * Test the sequential version of the Huffman encoding algorithm.
 */

#include <iostream>

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
        cout << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
    string filename = argv[1];

    // STEP 1: Read the input file
    string text;
    {
        utimer t("Read file (seq)");
        text = io_file::readSeq("tests/inputs/" + filename);
    }

    // STEP 2: Compute the frequency of each character
    vector<int> *charsFrequency;
    {
        utimer t("Chars frequency (seq)");
        charsFrequency = chars_frequency::computeSeq(text, 0, text.length());
    }

    // STEP 3-4: Build the Huffman tree and the Huffman map
    vector<string> *huffmanMap;
    {
        utimer t("Huffman tree and map (seq)");
        huffman_tree::Node *root = huffman_tree::buildHuffmanTree(text, charsFrequency);
        huffmanMap = huffman_tree::buildHuffmanMap(root);

        delete charsFrequency;
        huffman_tree::deleteHuffmanTree(root);
    }

    // STEP 5: Encode the text using the Huffman map
    string binaryString;
    {
        utimer t("Encode text (seq)");
        binaryString = huffman_encode::fromStringToBinarySeq(text, huffmanMap);

        delete huffmanMap;
    }

    // STEP 6: Convert the binary string to an ASCII string
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

    return 0;
}