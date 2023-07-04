/**
 * @file read_from_file_seq.hpp
 *
 * @brief Read from file sequentially.
 *
 * Read from file sequentially.
 */

#ifndef IO_FILE_SEQ_HPP
#define IO_FILE_SEQ_HPP

#include <string>

using namespace std;

/**
 * @brief Read from file sequentially.
 *
 * Read from file sequentially.
 *
 * @param filePath The path of the file to read from.
 * @return The content of the file.
 */
string readFromFileSeq(string filePath);

/**
 * @brief Write to file sequentially.
 *
 * Write to file sequentially.
 *
 * @param filePath The path of the file to write to.
 * @param text The text to write to the file.
 * @return void
 */
void writeToFileSeq(string filePath, string text);

#endif // IO_FILE_SEQ_HPP