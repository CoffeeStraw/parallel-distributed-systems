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

vector<int> computeMultiThreadedLockVector(const string &text, const int nWorkers)
{
    vector<int> result = vector<int>(256, 0);

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

    // Merge
    for (int i = 0; i < nWorkers; i++)
        threads[i].join();

    return result;
}

vector<int> computeMultiThreadedLockElement(const string &text, const int nWorkers)
{
    vector<int> result = vector<int>(256, 0);

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

    // Merge
    for (int i = 0; i < nWorkers; i++)
        threads[i].join();

    return result;
}

vector<int> computeFastFlowForReduce(const string &text, const int nWorkers)
{
    vector<int> result = vector<int>(256, 0);

    ff::ParallelForReduce<vector<int>> ffForReduce;
    ffForReduce.parallel_reduce_static(
        result, vector<int>(256, 0),
        0, text.size(), 1, 0,
        [&text](const long i, vector<int> &sub_result)
        {
            // Counting
            int pos = static_cast<unsigned char>(text[i]);
            sub_result[pos]++;
        },
        [](vector<int> &result, const vector<int> &sub_result)
        {
            // Merging
            for (int i = 0; i < 256; i++)
                result[i] += sub_result[i];
        },
        nWorkers);

    return result;
}

int main(int argc, char *argv[])
{
    // Read command line arguments
    if (argc != 4)
    {
        cout << "Usage: " << argv[0] << " <filename> <nWorkers> <nIterations)" << endl;
        return 1;
    }
    string filename = argv[1];
    int nWorkers = atoi(argv[2]);
    int nIterations = atoi(argv[3]);

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
    
    // FastFlow execution with reduce
    us, averageUs = 0;
    for (int i = 0; i < nIterations; i++)
    {
        {
            utimer t("", &us);
            charsFrequency = computeFastFlowForReduce(text, nWorkers);
        }
        averageUs += us;
    }
    averageUs /= nIterations;
    cout << "Average FastFlow execution time with reduce: " << averageUs << " us" << endl;

    return 0;
}