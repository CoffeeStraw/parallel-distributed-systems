# Parallel Huffman Coding
An implementation of Huffman Coding in C++ using threads and FastFlow.

## Steps of the algorithm
1. READ: Read the input file and store it into a string object;
2. COUNT: Count the frequency of each character;
3. TREE: Build the Huffman tree;
4. MAP: Build the Huffman map (char->code);
5. ENCODE: Encode the input file using Huffman map;
6. ASCII: Encode the obtained binary string in ASCII;
7. WRITE: Write the encoded file;

## Compile
Sequential version:
```
g++ -O3 -I /usr/local/include -std=c++17 src/*.cpp tests/test_1_sequential.cpp  -o main
```

Thread version:
```
g++ -O3 -I /usr/local/include -std=c++17 src/*.cpp tests/test_2_thread.cpp  -o main
```

FastFlow version:
```
g++ -O3 -I /usr/local/include -std=c++17 src/*.cpp tests/test_3_fastflow.cpp  -o main
```