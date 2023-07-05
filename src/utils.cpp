/**
 * @file utils.cpp
 *
 * @brief Utility functions.
 *
 * Utility functions.
 *
 * @see utils.hpp
 */

#include <iostream>
#include <string>

#include "utils.hpp"

using namespace std;
using namespace huffman_tree;

void printHuffmanTreeRec(Node *root, string pathString)
{
    if (root == nullptr)
        return;

    if (root->character != '$')
        cout << root->character << ": " << pathString << endl;

    printHuffmanTreeRec(root->left, pathString + "0");
    printHuffmanTreeRec(root->right, pathString + "1");
}

void utils::printHuffmanTree(Node *root)
{
    printHuffmanTreeRec(root, "");
}