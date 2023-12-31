/**
 * @file utils.hpp
 *
 * @brief Utility functions.
 *
 * Utility functions.
 *
 * @see utils.cpp
 */

#include <iostream>
#include <fstream>

#include "huffman_tree.hpp"

using namespace std;
using namespace huffman_tree;

#ifndef UTILS_HPP
#define UTILS_HPP

namespace utils
{
    /**
     * @brief Print the Huffman tree.
     *
     * @details Print the Huffman tree in a human-readable format.
     *
     * @param struct Node* Root of the Huffman tree.
     */
    void printHuffmanTree(Node *root);
}
#endif