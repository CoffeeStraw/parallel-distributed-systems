/**
 * Test the multi-threaded version of the Huffman encoding algorithm.
 */

#include <iostream>

#include "../src/include/io_file.hpp"
#include "../src/include/chars_frequency.hpp"

using namespace std;

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

    // Run required steps to reach the COUNT stage
    string text = io_file::readSeq("tests/inputs/" + filename);

    // Verify that the 3 functions compute the same result
    vector<int> charsFrequency1 = chars_frequency::computeSeq(text, 0, text.length());
    vector<int> charsFrequency2 = chars_frequency::computeMultiThreaded(text, nWorkers);
    vector<int> charsFrequency3 = chars_frequency::computeFastFlow(text, nWorkers);

    for (int i = 0; i < 256; i++)
    {
        if (charsFrequency1[i] != charsFrequency2[i] || charsFrequency1[i] != charsFrequency3[i])
        {
            cout << "Error: the three functions compute different results" << endl;
            return 1;
        }
    }

    return 0;
}