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
#include <vector>

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
     * @brief Create a new Huffman tree from the frequency of each character.
     * Pseudo-code: https://courses.cs.northwestern.edu/311/notes/huffman.pdf
     *
     * @param charsFrequency Map of characters to their frequency.
     * @return Node* Root of the Huffman tree.
     */
    Node *buildHuffmanTree(const vector<int> &charsFrequency);

    /**
     * @brief Delete the Huffman tree.
     *
     * @param root Root of the Huffman tree.
     */
    void deleteHuffmanTree(Node *root);

    /**
     * @brief Build a map of characters to Huffman codes.
     *
     * @param root Root of the Huffman tree.
     * @return Map of characters to Huffman codes.
     */
    vector<string> buildHuffmanMap(const Node *root);
}

#endif