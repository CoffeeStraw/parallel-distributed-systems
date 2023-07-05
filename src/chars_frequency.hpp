/**
 * @file chars_frequency.hpp
 * 
 * @brief Compute the frequency of each character in a string.
 */

#ifndef CHARS_FREQUENCY_HPP
#define CHARS_FREQUENCY_HPP

#include <string>
#include <unordered_map>

using namespace std;

/**
 * @brief Compute the frequency of each character in a string sequentially.
 * 
 * @details Compute the frequency of each character in a string sequentially.
 * 
 * @param text String to compute the frequency of each character.
 * @return unordered_map<char, int> Map of characters to their frequency.
 */
unordered_map<char, int>* computeCharsFrequencySeq(const string &text);

#endif