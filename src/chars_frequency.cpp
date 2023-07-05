#include "chars_frequency.hpp"

using namespace std;

unordered_map<char, int> *computeCharsFrequencySeq(const string &text)
{
    unordered_map<char, int> *charsFrequency = new unordered_map<char, int>();

    for (auto character : text)
        (*charsFrequency)[character]++;

    return charsFrequency;
}