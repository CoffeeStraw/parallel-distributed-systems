/**
 * @file huffman_encode.cpp
 *
 * @brief Huffman encoding functions.
 *
 * Huffman encoding functions.
 *
 * @see huffman_encode.h
 */

#include <iostream>

#include "huffman_encode.hpp"

using namespace std;

string huffmanEncode(string text)
{
    Node *root = buildHuffmanTree(text);
    // printHuffmanTree(root);

    unordered_map<char, string> huffmanMap;
    buildHuffmanMap(root, "", huffmanMap);

    string encodedText = "";
    for (auto character : text)
        encodedText += huffmanMap[character];
    return encodedText;
}