#include <iostream>
#include <fstream>

#include "../src/include/io_file.hpp"
#include "../src/include/utimer.hpp"

using namespace std;

string readSeqOneGo(string filePath)
{
    string text;
    string line;
    ifstream file(filePath, ios::in | ios::binary);

    if (file)
        return string(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
    else
        throw runtime_error("Unable to open file" + filePath + "\n");

    return text;
}

int main(int argc, char *argv[])
{
    string filename = argv[1];

    {
        utimer t("readSeq");
        // Execute it 100 times to get a more accurate time measurement
        for (int i = 0; i < 100; i++)
            string text = io_file::readSeq("tests/inputs/" + filename);
    }

    {
        utimer t("readSeqOneGo");
        // Execute it 100 times to get a more accurate time measurement
        for (int i = 0; i < 100; i++)
            string textNew = readSeqOneGo("tests/inputs/" + filename);
    }

    return 0;
}