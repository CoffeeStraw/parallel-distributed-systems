#include <vector>
#include <thread>
#include <mutex>
#include <iostream>

#include "chars_frequency.hpp"

using namespace std;

mutex mtx;

vector<int> *chars_frequency::computeSeq(const string &text, const int start, const int end)
{
    vector<int> *result = new vector<int>(256, 0); // Assuming ASCII characters (0-255 range), for Unicode it would be 0-65535

    for (int i = start; i < end; i++)
    {
        int pos = static_cast<unsigned char>(text[i]);
        (*result)[pos]++;
    }

    return result;
}

vector<int> *chars_frequency::computeMultiThreaded(const string &text, const int nWorkers)
{
    vector<vector<int> *> chunksResult(nWorkers);

    // Static load balancing: each thread gets a chunk of the text of the same size
    vector<thread> threads;
    int chunkSize = text.length() / nWorkers;
    int start = 0;
    int end = chunkSize;

    for (int i = 0; i < nWorkers; i++)
    {
        if (i == nWorkers - 1)
            end = text.length();

        threads.push_back(thread(
            [i, &text, start, end, &chunksResult]()
            {
                vector<int> *resultPartial = chars_frequency::computeSeq(text, start, end);
                chunksResult[i] = resultPartial;
            }));

        start = end;
        end += chunkSize;
    }

    // Merge
    vector<int> *result = new vector<int>(256, 0);
    for (int i = 0; i < nWorkers; i++)
    {
        threads[i].join();
        for (int j = 0; j < 256; j++)
            (*result)[j] += (*chunksResult[i])[j];
    }

    return result;
}