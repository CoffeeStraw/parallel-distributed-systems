/**
 * Compute the overhead of the chars_frequency function.
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>

#include "../src/include/io_file.hpp"
#include "../src/include/chars_frequency.hpp"
#include "../src/include/utimer.hpp"

using namespace std;

vector<int> computeMultiThreadedMeasureOverhead(const string &text, const int nWorkers)
{
    // Fill with vector of 256 elements initialized to 0, since this will not be done later
    vector<vector<int>> chunksResult(nWorkers);
    for (int i = 0; i < nWorkers; i++)
        chunksResult[i] = vector<int>(256, 0);

    // Static load balancing: each thread gets a chunk of the text of the same size
    vector<thread> threads;
    int chunkSize = text.length() / nWorkers;
    int start = 0;
    int end = chunkSize;

    // Spawn threads doing nothing
    for (int i = 0; i < nWorkers; i++)
    {
        if (i == nWorkers - 1)
            end = text.length();

        threads.push_back(thread([&]() {}));

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

int main(int argc, char *argv[])
{
    // Read command line arguments
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " <nWorkers> <nIterations>" << endl;
        return 1;
    }
    string filename = "100MB.txt";
    int nWorkers = atoi(argv[1]);
    int nIterations = atoi(argv[2]);

    // Run required steps to reach the COUNT stage
    string text = io_file::readSeq("tests/inputs/" + filename);
    vector<int> charsFrequency;

    // Multi-threaded execution
    long us, averageUs = 0;
    for (int i = 0; i < nIterations; i++)
    {
        {
            utimer t("", &us);
            charsFrequency = chars_frequency::computeMultiThreaded(text, nWorkers);
        }
        averageUs += us;
    }
    averageUs /= nIterations;
    cout << "Average multi-threaded execution time: " << averageUs << " us" << endl;

    // Multi-threaded execution (measure overhead)
    us, averageUs = 0;
    for (int i = 0; i < nIterations; i++)
    {
        {
            utimer t("", &us);
            charsFrequency = computeMultiThreadedMeasureOverhead(text, nWorkers);
        }
        averageUs += us;
    }
    averageUs /= nIterations;
    cout << "Average multi-threaded overhead time " << averageUs << " us" << endl;

    return 0;
}