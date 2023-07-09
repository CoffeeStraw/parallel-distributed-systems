/**
 * @file chars_frequency.hpp
 *
 * @brief Compute the frequency of each character in a string.
 */

#ifndef CHARS_FREQUENCY_HPP
#define CHARS_FREQUENCY_HPP

#include <string>
#include <vector>

using namespace std;

namespace chars_frequency
{
    /**
     * @brief Compute the frequency of each character in a string sequentially.
     *
     * @param text String to compute the frequency of each character.
     * @param start Start index of the string.
     * @param end End index of the string.
     * @return unordered_map<char, int> Map of characters to their frequency.
     */
    vector<int> computeSeq(const string &text, const int start, const int end);

    /**
     * @brief Compute the frequency of each character in a string using threads.
     *
     * @param text String to compute the frequency of each character.
     * @param nWorkers Number of workers to use.
     * @return vector<int>* Vector of characters to their frequency.
     */
    vector<int> computeMultiThreaded(const string &text, const int nWorkers);

    /**
     * @brief Compute the frequency of each character in a string using FastFlow.
     *
     * @param text String to compute the frequency of each character.
     * @param nWorkers Number of workers to use.
     * @return vector<int>* Vector of characters to their frequency.
     */
    vector<int> computeFastFlow(const string &text, const int nWorkers);
}

#endif