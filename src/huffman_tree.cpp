/**
 * @file huffman_tree.cpp
 *
 * @brief Huffman tree and node struct and functions.
 *
 * Huffman tree and node struct and functions.
 *
 * @see huffman_tree.h
 */

#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>

#include "huffman_tree.hpp"

using namespace std;

Node *createNode(char character, int frequency)
{
    Node *node = new Node{character, frequency};
    return node;
}

Node *buildHuffmanTree(string text)
{
    // Count characters' frequencies
    unordered_map<char, int> charsFrequency;
    for (auto character : text)
        charsFrequency[character]++;

    // Create a min heap with nodes containing characters and their frequencies
    // NOTE: this one maybe should be put in a different function, as it can be parallelized
    priority_queue<Node *, vector<Node *>, comp> minHeap;
    for (auto pair : charsFrequency)
        minHeap.push(createNode(pair.first, pair.second));

    // Create the Huffman tree
    while (minHeap.size() != 1)
    {
        // Extract the two minimum frequency items
        Node *left = minHeap.top();
        minHeap.pop();
        Node *right = minHeap.top();
        minHeap.pop();

        // Create a new node holding the sum of the frequencies of the two nodes
        Node *internalNode = createNode('$', left->frequency + right->frequency);
        internalNode->left = left;
        internalNode->right = right;

        // Insert the new internal node into the min heap
        minHeap.push(internalNode);
    }

    // TODO: Try giving in input an empty string and see what happens
    return minHeap.top();
}

void printHuffmanTree(Node *root, string text)
{
    if (root == nullptr)
        return;

    if (root->character != '$')
        cout << root->character << ": " << text << endl;

    printHuffmanTree(root->left, text + "0");
    printHuffmanTree(root->right, text + "1");
}

void printHuffmanTree(Node *root)
{
    printHuffmanTree(root, "");
}

void buildHuffmanMap(Node *root, string tmp, unordered_map<char, string> &huffmanMap)
{
    if (root == nullptr)
        return;

    if (root->character != '$')
        huffmanMap[root->character] = tmp;

    buildHuffmanMap(root->left, tmp + "0", huffmanMap);
    buildHuffmanMap(root->right, tmp + "1", huffmanMap);
}