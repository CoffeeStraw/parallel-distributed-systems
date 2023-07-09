#include <vector>
#include <thread>
#include <iostream>
#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>

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
    vector<vector<int>> chunksResult(nWorkers);

    // Static load balancing: each thread gets a chunk of the text of the same size
    ff::ParallelFor pf(nWorkers);
    int chunkSize = text.length() / nWorkers;

    pf.parallel_for_static(
        0, nWorkers, 1, 0,
        [chunkSize, nWorkers, &chunksResult, &text](const long i)
        {
            int start = i * chunkSize;
            int end = start + chunkSize;
            if (i == nWorkers - 1)
                end = text.length();
            chunksResult[i] = chars_frequency::computeSeq(text, start, end);
        });

    // Merging
    vector<int> result = vector<int>(256, 0);
    for (int i = 0; i < nWorkers; i++)
        for (int j = 0; j < 256; j++)
            result[j] += chunksResult[i][j];

    return result;
}