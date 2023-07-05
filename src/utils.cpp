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

#include "huffman_tree.hpp"

using namespace std;

void printHuffmanTree(Node *root, string pathString)
{
    if (root == nullptr)
        return;

    if (root->character != '$')
        cout << root->character << ": " << pathString << endl;

    printHuffmanTree(root->left, pathString + "0");
    printHuffmanTree(root->right, pathString + "1");
}

void printHuffmanTree(Node *root)
{
    printHuffmanTree(root, "");
}