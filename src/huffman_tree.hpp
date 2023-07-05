/**
 * @file huffman_tree.h
 *
 * @brief Huffman tree and node struct and functions.
 *
 * Huffman tree and node struct and functions.
 *
 */

#ifndef HUFFMAN_TREE_HPP
#define HUFFMAN_TREE_HPP

#include <string>
#include <unordered_map>

using namespace std;

namespace huffman_tree
{
    struct Node
    {
        char character;
        int frequency;
        struct Node *left, *right;

        Node(char character, int frequency) : character(character), frequency(frequency), left(nullptr), right(nullptr){};
    };

    struct NodeComp
    {
        bool operator()(Node *l, Node *r)
        {
            return l->frequency > r->frequency;
        }
    };

    /**
     * @brief Create a new node with the given character and frequency. Created for convenience.
     *
     * @param character Character to be stored in the node.
     * @param frequency Frequency of the character.
     * @return struct Node* Pointer to the new node.
     */
    struct Node *createNode(const char character, const int frequency);

    /**
     * @brief Create a new Huffman tree from a string and the frequency of each character.
     * Pseudo-code: https://courses.cs.northwestern.edu/311/notes/huffman.pdf
     *
     * @details Create a new Huffman tree from a string and the frequency of each character.
     *
     * @param text String to be encoded.
     * @param charsFrequency Map of characters to their frequency.
     * @return Node* Root of the Huffman tree.
     */
    Node *buildHuffmanTree(const string &text, const unordered_map<char, int> *charsFrequency);

    /**
     * @brief Build a map of characters to Huffman codes.
     *
     * @details Build a map of characters to Huffman codes.
     *
     * @param root Root of the Huffman tree.
     * @param huffmanMap Map of characters to Huffman codes.
     */
    void buildHuffmanMap(const Node *root, unordered_map<char, string> &huffmanMap);
}

#endif