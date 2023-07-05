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

namespace chars_frequency
{
    /**
     * @brief Compute the frequency of each character in a string sequentially.
     *
     * @param text String to compute the frequency of each character.
     * @return unordered_map<char, int> Map of characters to their frequency.
     */
    unordered_map<char, int> *computeSeq(const string &text, const int start, const int end);

    /**
     * @brief Compute the frequency of each character in a string using threads.
     *
     * @param text String to compute the frequency of each character.
     * @return unordered_map<char, int> Map of characters to their frequency.
     */
    unordered_map<char, int> *computeMultiThreaded(const string &text, const int nThreads);
}

#endif