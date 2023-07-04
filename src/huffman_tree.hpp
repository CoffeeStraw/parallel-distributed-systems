/**
 * @file huffman_tree.h
 *
 * @brief Huffman tree and node struct and functions.
 *
 * Huffman tree and node struct and functions.
 *
 */

#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

struct Node
{
    char character;
    int frequency;
    struct Node *left, *right;

    Node(char character, int frequency) : character(character), frequency(frequency), left(nullptr), right(nullptr){};
};

struct comp
{
    bool operator()(Node *l, Node *r)
    {
        return l->frequency > r->frequency;
    }
};

/**
 * @brief Create a new node.
 *
 * @details Create a new node with the given character and frequency. Created for convenience.
 *
 * @param character Character to be stored in the node.
 * @param frequency Frequency of the character.
 * @return struct Node* Pointer to the new node.
 */
struct Node *createNode(char character, int frequency);

/**
 * @brief Create a new tree.
 *
 * @details Create a new Huffman tree from a string. Pseudo-code: https://courses.cs.northwestern.edu/311/notes/huffman.pdf
 *
 * @param text String to create the tree from.
 * @return struct Node* Pointer to the root of the tree.
 */
struct Node *buildHuffmanTree(string text);

/**
 * @brief Print the Huffman tree.
 *
 * @details Print the Huffman tree in a human-readable format.
 *
 * @param struct Node* Root of the Huffman tree.
 */
void printHuffmanTree(Node *root);

/**
 * @brief Build a map of characters to Huffman codes.
 *
 * @details Build a map of characters to Huffman codes.
 *
 * @param root Root of the Huffman tree.
 * @param code Huffman code.
 * @param huffmanMap Map of characters to Huffman codes.
 */
void buildHuffmanMap(Node *root, string code, unordered_map<char, string> &huffmanMap);

#endif