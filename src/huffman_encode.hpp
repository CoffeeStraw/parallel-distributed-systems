/**
 * @file huffman_encode.h
 *
 * @brief Huffman encoding functions.
 *
 * Huffman encoding functions.
 */

#ifndef HUFFMAN_ENCODE_H
#define HUFFMAN_ENCODE_H

#include <iostream>

#include "huffman_tree.hpp"

using namespace std;

/**
 * @brief Encode a string using Huffman coding, returning a binary string.
 *
 * @details Encode a string using Huffman coding, returning a binary string.
 *
 * @param string String to be encoded.
 * @return string Binary string representing the encoded text.
 */
string huffmanEncode(string text);

#endif