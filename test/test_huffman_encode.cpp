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

using namespace std;

int main()
{
    string text = "Hello World!";
    string encodedText = huffmanEncode(text);
    cout << "Encoded text: " << encodedText << endl;
    return 0;
}