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
#include "../src/io_file.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    string filename = argv[1];
    
    string text = readFromFileSeq("tests/inputs/" + filename);
    string encodedText = huffmanEncode(text);
    writeToFileSeq("tests/outputs/" + filename, encodedText);

    return 0;
}