#include <iostream>
#include <fstream>

#include "include/io_file.hpp"

using namespace std;

string io_file::readSeq(string filePath)
{
    string text;
    ifstream file(filePath);
    if (file.is_open())
        file >> text;
    else
        throw runtime_error("Unable to open file");
    return text;
}

void io_file::writeSeq(string filePath, string text)
{
    ofstream file(filePath);
    if (file.is_open())
        file << text;
    else
        throw runtime_error("Unable to open file");
}