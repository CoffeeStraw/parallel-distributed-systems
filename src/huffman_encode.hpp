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
 * @brief Encode a string using Huffman coding.
 *
 * @details Encode a string using Huffman coding.
 *
 * @param string String to be encoded.
 * @return string Encoded string.
 */
string huffmanEncode(string text);

#endif