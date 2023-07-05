# Parallel Huffman Coding
An implementation of Huffman Coding in C++ using threads and FastFlow.

## Steps of the algorithm
1. Read the input file and store it into a string object;
2. Count the frequency of each character;
3. Build the Huffman tree;
4. Build the Huffman map (char->code);
5. Encode the input file using Huffman map;
6. Encode the obtained binary string in ASCII;
7. Write the encoded file;

## Ideas
- Everything could be put inside a pipeline;
- Word-based vs char-based;
- If we encode using words, we could use a trie to store the words and their frequencies;
- Only for words, it could be worth parallelizing the insertion of the words in the min-heap;
- It would probably be more space-efficient to not encode using strings/chars, but rather using bytes/bits. However, new technical issues pop out: https://stackoverflow.com/questions/31796662/is-there-any-way-to-parallelize-huffman-encoding-implementation-on-hardware;
- While reading, we could already start counting the frequencies;

## Why not word-based Huffman Coding?
Huffman coding can be more efficient when applied at the character level or the word level, depending on the specific context and characteristics of the data being compressed. The efficiency of Huffman coding is influenced by factors such as the frequency distribution of characters or words in the data.

At the character level, Huffman coding assigns shorter codes to more frequently occurring characters, resulting in efficient compression for data with a high degree of character repetition. This can be particularly effective for compressing text files or data with a limited character set.

On the other hand, at the word level, Huffman coding assigns shorter codes to more frequently occurring words. This can be advantageous for compressing natural language text, where certain words or phrases may occur frequently. By encoding common words with shorter codes, the overall compression ratio can be improved.

Ultimately, the efficiency of Huffman coding, whether applied at the character or word level, depends on the specific characteristics of the data being compressed. It is important to analyze the frequency distribution of characters or words in the data to determine the most effective approach for Huffman coding.

## Usage

Sequential version:
```
g++ src/*.cpp test/test_huffman_encode.cpp -o main
```