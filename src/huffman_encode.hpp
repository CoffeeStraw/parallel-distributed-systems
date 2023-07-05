/**
 * @file huffman_encode.h
 *
 * @brief Methods to compute the Huffman encoding of a string.
 */

#ifndef HUFFMAN_ENCODE_HPP
#define HUFFMAN_ENCODE_HPP

#include <string>

using namespace std;

/**
 * @brief Encode a string using Huffman coding.
 *
 * @param string String to be encoded.
 * @return string ASCII string representing the encoded text.
 */
string huffmanEncode(const string &text);

#endif