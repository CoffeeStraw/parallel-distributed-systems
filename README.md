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

## TODO
- per charsfrequency, anziché mutex usare un array di maps e poi fare la join, discutere nel report;
- Vector instead of map;
- Per eseguire fastflow: `g++ -O3 -I /usr/local/include -std=c++17 filename.cpp -o …`
- CALCOLA OVERHEAD PER REPORT: lancia un thread a vuoto, fai una media di quanto tempo ci mette a partire;
- Fai delete del tree (anche se lo spazio occupato é basso);
- Secondo me fromStringToBinarySeq e fromBinaryToASCII possono essere composte in un'unica funzione;
- StringBuilder potrebbe migliorare performance?
- Si potrebbe cominciare a comporre mano a mano che i thread finiscono?
- Non ritornare la stringa e basta, altrimenti credo che venga copiata;
- Menzionare nel report il return di stringhe ottimizzato: https://en.cppreference.com/w/cpp/language/copy_elision;

## Ideas
- Could it make sense to use thread-pools?;
- Everything could be put inside a pipeline;
- Word-based vs char-based;
- If we encode using words, we could use a trie to store the words and their frequencies?;
- Only for words, it could be worth parallelizing the insertion of the words in the min-heap;
- It would probably be more space-efficient to not encode using strings/chars, but rather using bytes/bits. However, new technical issues pop out: https://stackoverflow.com/questions/31796662/is-there-any-way-to-parallelize-huffman-encoding-implementation-on-hardware;
- While reading, we could already start counting the frequencies;

## Why not counting while reading?
In the sequential version, counting char-by-char would be inefficient.
In the parallel version, we could read in chunk and spawn a thread for counting as soon as we have a chunk. Since we are also reading sequentially.

## Why not word-based Huffman Coding?
Huffman coding can be more efficient when applied at the character level or the word level, depending on the specific context and characteristics of the data being compressed. The efficiency of Huffman coding is influenced by factors such as the frequency distribution of characters or words in the data.

At the character level, Huffman coding assigns shorter codes to more frequently occurring characters, resulting in efficient compression for data with a high degree of character repetition. This can be particularly effective for compressing text files or data with a limited character set.

On the other hand, at the word level, Huffman coding assigns shorter codes to more frequently occurring words. This can be advantageous for compressing natural language text, where certain words or phrases may occur frequently. By encoding common words with shorter codes, the overall compression ratio can be improved.

Ultimately, the efficiency of Huffman coding, whether applied at the character or word level, depends on the specific characteristics of the data being compressed. It is important to analyze the frequency distribution of characters or words in the data to determine the most effective approach for Huffman coding.

## Usage

Sequential version:
```
g++ src/*.cpp tests/test_seq.cpp -o main
```