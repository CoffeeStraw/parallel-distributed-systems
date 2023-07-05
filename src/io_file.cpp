#include <iostream>
#include <fstream>

#include "io_file.hpp"

using namespace std;

string io_file::readSeq(string filePath)
{
    string text;
    string line;
    ifstream file(filePath);

    if (file.is_open())
    {
        while (getline(file, line))
            text += line + "\n";
        file.close();
    }
    else
        cout << "Unable to open file" << endl;
    return text;
}

void io_file::writeSeq(string filePath, string text)
{
    ofstream file(filePath);
    if (file.is_open())
    {
        file << text;
        file.close();
    }
    else
        cout << "Unable to open file" << endl;
}