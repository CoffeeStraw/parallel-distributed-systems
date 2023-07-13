#include <fstream>
#include <sstream>

#include "include/io_file.hpp"

using namespace std;

string io_file::readSeq(const string &filePath)
{
    std::ifstream file(filePath);
    if (file.is_open())
    {
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    else
        throw std::runtime_error("Unable to open file");
}

void io_file::writeSeq(const string &filePath, const string &text)
{
    ofstream file(filePath);
    if (file.is_open())
        file << text;
    else
        throw runtime_error("Unable to open file");
}