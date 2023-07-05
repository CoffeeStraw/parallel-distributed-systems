/**
 * @file huffman_encode.h
 *
 * @brief Methods to compute the Huffman encoding of a string.
 */

#ifndef HUFFMAN_ENCODE_HPP
#define HUFFMAN_ENCODE_HPP

#include <string>
#include <unordered_map>

using namespace std;

namespace huffman_encode
{
    /**
     * @brief Computes the Huffman encoding of a string.
     * 
     * @param text The string to encode.
     * @param huffmanMap The Huffman map (char->code).
     * @return A binary string representing the encoded text (i.e. a string of 0s and 1s).
    */
    string fromStringToBinarySeq(const string &text, unordered_map<char, string> &huffmanMap);

    /**
     * @brief Computes the Huffman encoding of a string using multiple threads.
     * 
     * @param text The string to encode.
     * @param huffmanMap The Huffman map (char->code).
     * @param nWorkers The number of threads to use.
     * @return A binary string representing the encoded text (i.e. a string of 0s and 1s).
    */
    string fromStringToBinaryMultiThreaded(const string &text, unordered_map<char, string> &huffmanMap, int nWorkers);

    /**
     * @brief Converts a binary string to an ASCII string.
     * 
     * @param binaryString The binary string to convert.
     * @return An ASCII string.
    */
    string fromBinaryToASCIISeq(string binaryString);
}

#endif