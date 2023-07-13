/**
 * Test the multi-threaded version of the Huffman encoding algorithm.
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

std::mutex mtx;
std::vector<std::mutex> mutexes(256);

/**
 * Same as chars_frequency::computeMultiThreaded,
 * but threads autonomously write the results by locking the vector
 */
vector<int> computeMultiThreadedLockVector(const string &text, const int nWorkers)
{
    vector<int> result = vector<int>(256, 0);

    vector<thread> threads;
    int chunkSize = text.length() / nWorkers;
    int start = 0;
    int end = chunkSize;

    for (int i = 0; i < nWorkers; i++)
    {
        if (i == nWorkers - 1)
            end = text.length();

        threads.push_back(thread(
            [i, &text, start, end, &result]()
            {
                vector<int> resultPartial = chars_frequency::computeSeq(text, start, end);

                lock_guard<mutex> lock(mtx);
                for (int j = 0; j < 256; j++)
                    result[j] += resultPartial[j];
            }));

        start = end;
        end += chunkSize;
    }

    for (int i = 0; i < nWorkers; i++)
        threads[i].join();

    return result;
}

/**
 * Same as chars_frequency::computeMultiThreaded,
 * but threads autonomously write the results by locking elements of the vector
 */
vector<int> computeMultiThreadedLockElement(const string &text, const int nWorkers)
{
    vector<int> result = vector<int>(256, 0);

    vector<thread> threads;
    int chunkSize = text.length() / nWorkers;
    int start = 0;
    int end = chunkSize;

    for (int i = 0; i < nWorkers; i++)
    {
        if (i == nWorkers - 1)
            end = text.length();

        threads.push_back(thread(
            [i, &text, start, end, &result]()
            {
                vector<int> resultPartial = chars_frequency::computeSeq(text, start, end);
                for (int j = 0; j < 256; j++)
                {
                    lock_guard<mutex> lock(mutexes[j]);
                    result[j] += resultPartial[j];
                }
            }));

        start = end;
        end += chunkSize;
    }

    for (int i = 0; i < nWorkers; i++)
        threads[i].join();

    return result;
}

/**
 * Same as chars_frequency::computeFastFlow,
 * but the reduce phase is performed out of the library control
 */
vector<int> computeFastFlowNoReduce(const string &text, const int nWorkers)
{
    vector<vector<int>> chunksResult(nWorkers);

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

    vector<int> result = vector<int>(256, 0);
    for (int i = 0; i < nWorkers; i++)
        for (int j = 0; j < 256; j++)
            result[j] += chunksResult[i][j];

    return result;
}

/**
 * Same as chars_frequency::computeFastFlow,
 * but using dynamic scheduling
 */
vector<int> computeFastFlowNoStatic(const string &text, const int nWorkers)
{
    vector<int> result = vector<int>(256, 0);

    ff::ParallelForReduce<vector<int>> ffForReduce(nWorkers);
    ffForReduce.parallel_reduce(
        result, vector<int>(256, 0),
        0, text.size(), 1, 0,
        [&text](const long i, vector<int> &partialResult)
        {
            int pos = static_cast<unsigned char>(text[i]);
            partialResult[pos]++;
        },
        [](vector<int> &result, const vector<int> &partialResult)
        {
            for (int i = 0; i < 256; i++)
                result[i] += partialResult[i];
        },
        nWorkers);

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

    // Seqeuential execution
    long us, averageUs = 0;
    for (int i = 0; i < nIterations; i++)
    {
        {
            utimer t("", &us);
            charsFrequency = chars_frequency::computeSeq(text, 0, text.length());
        }
        averageUs += us;
    }
    averageUs /= nIterations;
    cout << "Average sequential execution time: " << averageUs << " us" << endl;

    // Multi-threaded execution
    us, averageUs = 0;
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

    // Multi-threaded execution with lock on vector
    us, averageUs = 0;
    for (int i = 0; i < nIterations; i++)
    {
        {
            utimer t("", &us);
            charsFrequency = computeMultiThreadedLockVector(text, nWorkers);
        }
        averageUs += us;
    }
    averageUs /= nIterations;
    cout << "Average multi-threaded execution time with lock on vector: " << averageUs << " us" << endl;

    // Multi-threaded execution with lock on element
    us, averageUs = 0;
    for (int i = 0; i < nIterations; i++)
    {
        {
            utimer t("", &us);
            charsFrequency = computeMultiThreadedLockElement(text, nWorkers);
        }
        averageUs += us;
    }
    averageUs /= nIterations;
    cout << "Average multi-threaded execution time with lock on element: " << averageUs << " us" << endl;

    // FastFlow execution
    us, averageUs = 0;
    for (int i = 0; i < nIterations; i++)
    {
        {
            utimer t("", &us);
            charsFrequency = chars_frequency::computeFastFlow(text, nWorkers);
        }
        averageUs += us;
    }
    averageUs /= nIterations;
    cout << "Average FastFlow execution time: " << averageUs << " us" << endl;

    // FastFlow execution with no reduce
    us, averageUs = 0;
    for (int i = 0; i < nIterations; i++)
    {
        {
            utimer t("", &us);
            charsFrequency = computeFastFlowNoReduce(text, nWorkers);
        }
        averageUs += us;
    }
    averageUs /= nIterations;
    cout << "Average FastFlow execution time with no reduce: " << averageUs << " us" << endl;

    // FastFlow execution with no static
    us, averageUs = 0;
    for (int i = 0; i < nIterations; i++)
    {
        {
            utimer t("", &us);
            charsFrequency = computeFastFlowNoStatic(text, nWorkers);
        }
        averageUs += us;
    }
    averageUs /= nIterations;
    cout << "Average FastFlow execution time with no static: " << averageUs << " us" << endl;

    return 0;
}