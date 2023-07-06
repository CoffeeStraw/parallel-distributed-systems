#include <queue>

#include "huffman_tree.hpp"

using namespace std;
using namespace huffman_tree;

Node *huffman_tree::createNode(const char character, const int frequency)
{
    Node *node = new Node{character, frequency};
    return node;
}

Node *huffman_tree::buildHuffmanTree(const string &text, const vector<int> *charsFrequency)
{
    if (text.empty())
        return nullptr;

    // Create a min-heap with nodes containing characters and their frequencies
    priority_queue<Node *, vector<Node *>, NodeComp> minHeap;
    for (int i = 0; i < charsFrequency->size(); i++)
        minHeap.push(
            createNode(
                static_cast<char>(i),
                (*charsFrequency)[i]));

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

void buildHuffmanMapRec(const Node *root, const string tmp, vector<string> *huffmanMap)
{
    if (root == nullptr)
        return;

    if (root->character != '$')
        (*huffmanMap)[static_cast<unsigned char>(root->character)] = tmp;

    buildHuffmanMapRec(root->left, tmp + "0", huffmanMap);
    buildHuffmanMapRec(root->right, tmp + "1", huffmanMap);
}

vector<string>* huffman_tree::buildHuffmanMap(const Node *root)
{
    vector<string> *huffmanMap = new vector<string>(256);
    buildHuffmanMapRec(root, "", huffmanMap);
    return huffmanMap;
}