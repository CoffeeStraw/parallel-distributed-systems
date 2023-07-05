/**
 * Test the multi-threaded version of the Huffman encoding algorithm.
*/

#include <iostream>

#include "../src/io_file.hpp"
#include "../src/chars_frequency.hpp"

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
    unordered_map<char, int> *charsFrequency1 = chars_frequency::computeSeq(text, 0, text.length());
    unordered_map<char, int> *charsFrequency2 = chars_frequency::computeMultiThreaded(text, nWorkers);

    if (*charsFrequency1 != *charsFrequency2)
    {
        cout << "ERROR: the two maps have different content" << endl;
        return 1;
    }

    return 0;
}