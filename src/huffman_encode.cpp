#include <iostream>

#include "huffman_encode.hpp"
#include "huffman_tree.hpp"
#include "chars_frequency.hpp"
// #include "utils.hpp"

using namespace std;

string huffmanEncode(const string &text)
{
    unordered_map<char, int>* charsFrequency = computeCharsFrequencySeq(text);

    Node *root = buildHuffmanTree(text, charsFrequency);
    // printHuffmanTree(root);

    unordered_map<char, string> huffmanMap;
    buildHuffmanMap(root, "", huffmanMap);

    string encodedText = "";
    for (auto character : text)
        encodedText += huffmanMap[character];
    return encodedText;
}