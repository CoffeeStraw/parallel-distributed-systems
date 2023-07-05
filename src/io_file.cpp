#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>

#include "io_file.hpp"

using namespace std;

string readFromFileSeq(string filePath)
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

string binaryStringToASCIIString(string binaryString)
{
    // Pad the binary string with 0s at the end, to make its length a multiple of 8
    int remainder = binaryString.length() % 8;
    if (remainder != 0)
        binaryString.append(8 - remainder, '0');

    // Convert the binary string to an ASCII string
    string asciiString = "";
    stringstream sstream(binaryString);
    while (sstream.good())
    {
        bitset<8> bits;
        sstream >> bits;
        asciiString += char(bits.to_ulong());
    }
    return asciiString;
}

void writeToFileSeq(string filePath, string binaryText)
{
    string asciiText = binaryStringToASCIIString(binaryText);
    ofstream file(filePath);
    if (file.is_open())
    {
        file << asciiText;
        file.close();
    }
    else
        cout << "Unable to open file" << endl;
}