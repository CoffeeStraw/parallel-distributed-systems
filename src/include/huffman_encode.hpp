/**
 * @file huffman_encode.h
 *
 * @brief Methods to compute the Huffman encoding of a string.
 */

#ifndef HUFFMAN_ENCODE_HPP
#define HUFFMAN_ENCODE_HPP

#include <string>
#include <vector>

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
    string fromStringToBinarySeq(const string &text, const int start, const int end, const vector<string> &huffmanMap);

    /**
     * @brief Computes the Huffman encoding of a string using multiple threads.
     *
     * @param text The string to encode.
     * @param huffmanMap The Huffman map (char->code).
     * @param nWorkers The number of threads to use.
     * @return A binary string representing the encoded text (i.e. a string of 0s and 1s).
     */
    string fromStringToBinaryMultiThreaded(const string &text, const vector<string> &huffmanMap, const int nWorkers);

    /**
     * @brief Computes the Huffman encoding of a string using FastFlow.
     * 
     * @param text The string to encode.
     * @param huffmanMap The Huffman map (char->code).
     * @param nWorkers The number of workers to use.
    */
    string fromStringToBinaryFastFlow(const string &text, const vector<string> &huffmanMap, const int nWorkers);

    /**
     * @brief Pad the binary string with 0s at the end, to make its length a multiple of 8.
     *
     * @param binaryString The binary string to pad.
     */
    void padString(string &binaryString);

    /**
     * @brief Converts a binary string to an ASCII string.
     *
     * @param binaryString The binary string to convert.
     * @return An ASCII string.
     */
    string fromBinaryToASCIISeq(const string &binaryString, const int start, const int end);

    /**
     * @brief Converts a binary string to an ASCII string using multiple threads.
     *
     * @param binaryString The binary string to convert.
     * @param nWorkers The number of threads to use.
     * @return An ASCII string.
     */
    string fromBinaryToASCIIMultiThreaded(const string &binaryString, const int nWorkers);

    /**
     * @brief Converts a binary string to an ASCII string using FastFlow.
     * 
     * @param binaryString The binary string to convert.
     * @param nWorkers The number of workers to use.
     * @return An ASCII string.
    */
    string fromBinaryToASCIIFastFlow(const string &binaryString, const int nWorkers);
}

#endif