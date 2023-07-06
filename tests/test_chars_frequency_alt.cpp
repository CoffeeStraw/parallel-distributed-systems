/**
 * Test the multi-threaded version of the Huffman encoding algorithm.
 */

#include <iostream>
#include <thread>
#include <mutex>

#include "../src/io_file.hpp"
#include "../src/chars_frequency.hpp"
#include "../src/utimer.cpp"

using namespace std;

std::mutex mtx;
std::vector<std::mutex> mutexes(256);

vector<int> *computeMultiThreadedLockVector(const string &text, const int nWorkers)
{
    vector<int> *result = new vector<int>(256, 0);

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
                vector<int> *resultPartial = chars_frequency::computeSeq(text, start, end);
                
                lock_guard<mutex> lock(mtx);
                for (int j = 0; j < 256; j++)
                    (*result)[j] += (*resultPartial)[j];
            }));

        start = end;
        end += chunkSize;
    }

    // Merge
    for (int i = 0; i < nWorkers; i++)
        threads[i].join();

    return result;
}

vector<int> *computeMultiThreadedLockElement(const string &text, const int nWorkers)
{
    vector<int> *result = new vector<int>(256, 0);

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
                vector<int> *resultPartial = chars_frequency::computeSeq(text, start, end);
                for (int j = 0; j < 256; j++)
                {
                    lock_guard<mutex> lock(mutexes[j]);
                    (*result)[j] += (*resultPartial)[j];
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

int main(int argc, char *argv[])
{
    // Read command line arguments
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " <filename> <nWorkers>" << endl;
        return 1;
    }
    string filename = argv[1];
    int nWorkers = atoi(argv[2]);

    string text = io_file::readSeq("tests/inputs/" + filename);

    // Verify that the two functions compute the same result
    vector<int> *charsFrequency1;
    {
        utimer t("computeMultiThreaded");
        // Execute it 100 times to get a more accurate time measurement
        for (int i = 0; i < 100; i++)
            charsFrequency1 = chars_frequency::computeMultiThreaded(text, nWorkers);
    }
    vector<int> *charsFrequency2;
    {
        utimer t("computeMultiThreadedLockVector");
        // Execute it 100 times to get a more accurate time measurement
        for (int i = 0; i < 100; i++)
            charsFrequency2 = computeMultiThreadedLockVector(text, nWorkers);
    }
    vector<int> *charsFrequency3;
    {
        utimer t("computeMultiThreadedLockElement");
        // Execute it 100 times to get a more accurate time measurement
        for (int i = 0; i < 100; i++)
            charsFrequency3 = computeMultiThreadedLockElement(text, nWorkers);
    }

    for (int i = 0; i < 256; i++)
    {
        if ((*charsFrequency1)[i] != (*charsFrequency2)[i])
        {
            cout << "Error: the two functions compute different results" << endl;
            return 1;
        }
    }

    return 0;
}