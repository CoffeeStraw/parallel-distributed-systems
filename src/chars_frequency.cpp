#include <vector>
#include <thread>
#include <mutex>
#include <iostream>

#include "chars_frequency.hpp"

using namespace std;

mutex mtx;

unordered_map<char, int> *chars_frequency::computeSeq(const string &text, const int start, const int end)
{
    unordered_map<char, int> *result = new unordered_map<char, int>();

    for (int i = start; i < end; i++)
        (*result)[text[i]]++;

    return result;
}

unordered_map<char, int> *chars_frequency::computeMultiThreaded(const string &text, const int nThreads)
{
    unordered_map<char, int> *result = new unordered_map<char, int>();

    // Static load balancing: each thread gets a chunk of the text of the same size
    vector<thread> threads;
    int chunkSize = text.length() / nThreads;
    int start = 0;
    int end = chunkSize;

    for (int i = 0; i < nThreads; i++)
    {
        if (i == nThreads - 1)
            end = text.length();

        threads.push_back(thread(
            [text, start, end, result]()
            {
                unordered_map<char, int> *resultPartial = chars_frequency::computeSeq(text, start, end);

                mtx.lock();
                for (auto &pair : *resultPartial)
                    (*result)[pair.first] += pair.second;
                mtx.unlock();

                delete resultPartial;
            }));

        start = end;
        end += chunkSize;
    }

    for (auto &thread : threads)
        thread.join();

    return result;
}