#include <string>
#include <unordered_map>
#include <queue>

#include "huffman_tree.hpp"

using namespace std;

#include <iostream>

Node *createNode(const char character, const int frequency)
{
    Node *node = new Node{character, frequency};
    return node;
}

Node *buildHuffmanTree(const string &text, const unordered_map<char, int> *charsFrequency)
{
    if (text.empty())
        return nullptr;

    // Create a min-heap with nodes containing characters and their frequencies
    priority_queue<Node *, vector<Node *>, comp> minHeap;
    for (auto &pair : *charsFrequency)
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

    return minHeap.top();
}

void buildHuffmanMap(const Node *root, const string tmp, unordered_map<char, string> &huffmanMap)
{
    if (root == nullptr)
        return;

    if (root->character != '$')
        huffmanMap[root->character] = tmp;

    buildHuffmanMap(root->left, tmp + "0", huffmanMap);
    buildHuffmanMap(root->right, tmp + "1", huffmanMap);
}