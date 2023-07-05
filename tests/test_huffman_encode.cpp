/**
 * @file test_huffman_encode.cpp
 * 
 * @brief Test the Huffman encoding functions.
 * 
 * Test the Huffman encoding functions.
*/

#include <iostream>
#include <string>

#include "../src/huffman_encode.hpp"
#include "../src/io_file_seq.hpp"

using namespace std;

int main()
{
    string text = readFromFileSeq("test/inputs/1Mb.txt");
    string encodedText = huffmanEncode(text);
    writeToFileSeq("test/outputs/1Mb_encoded.txt", encodedText);

    return 0;
}