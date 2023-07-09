#include <vector>
#include <thread>
#include <iostream>
#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>
#include <time.h>

#include "include/chars_frequency.hpp"

using namespace std;

vector<int> chars_frequency::computeSeq(const string &text, const int start, const int end)
{
    vector<int> result = vector<int>(256, 0); // Assuming ASCII characters (0-255 range), for Unicode it would be 0-65535

    for (int i = start; i < end; i++)
    {
        int pos = static_cast<unsigned char>(text[i]);
        result[pos]++;
    }

    return result;
}

vector<int> chars_frequency::computeMultiThreaded(const string &text, const int nWorkers)
{
    vector<vector<int>> chunksResult(nWorkers);

    // Static load balancing: each thread gets a chunk of the text of the same size
    vector<thread> threads;
    int chunkSize = text.length() / nWorkers;
    int start = 0;
    int end = chunkSize;

    // Counting
    for (int i = 0; i < nWorkers; i++)
    {
        if (i == nWorkers - 1)
            end = text.length();

        threads.push_back(thread(
            [&chunksResult, i, &text, start, end]()
            {
                chunksResult[i] = chars_frequency::computeSeq(text, start, end);
            }));

        start = end;
        end += chunkSize;
    }

    // Merging
    vector<int> result = vector<int>(256, 0);
    for (int i = 0; i < nWorkers; i++)
    {
        threads[i].join();
        for (int j = 0; j < 256; j++)
            result[j] += chunksResult[i][j];
    }

    return result;
}

vector<int> chars_frequency::computeFastFlow(const string &text, const int nWorkers)
{
    vector<int> result = vector<int>(256, 0);

    ff::ParallelForReduce<vector<int>> ffForReduce(nWorkers);
    ffForReduce.parallel_reduce_static(
        result, vector<int>(256, 0),
        0, text.size(), 1, 0,
        [&text](const long i, vector<int> &partialResult)
        {
            // Counting
            int pos = static_cast<unsigned char>(text[i]);
            partialResult[pos]++;
        },
        [](vector<int> &result, const vector<int> &partialResult)
        {
            // Merging
            for (int i = 0; i < 256; i++)
                result[i] += partialResult[i];
        },
        nWorkers);

    return result;
}