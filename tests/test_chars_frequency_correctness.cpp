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

    string text = io_file::readSeq("tests/inputs/" + filename);

    // Verify that the two functions compute the same result
    vector<int> *charsFrequency1 = chars_frequency::computeSeq(text, 0, text.length());
    vector<int> *charsFrequency2 = chars_frequency::computeMultiThreaded(text, nWorkers);

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