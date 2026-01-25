# CPP-BPE-Tokenizer

A lightweight, efficient C++ implementation of Byte-Pair Encoding (BPE) for tokenization, suitable for training transformer models.

## Features

- **Byte-Level BPE**: Handles any text input by operating at the byte level.
- **Training (Fit)**: Learns a vocabulary and merge rules from a given text corpus.
- **Encoding**: Tokenizes text into a sequence of token IDs based on the learned vocabulary.
- **Decoding**: Reconstructs the original text from a sequence of token IDs.
- **Serialization**: Saves and loads the vocabulary (`vocab.json`) and merge rules (`merges.txt`) automatically.

## Prerequisites

- **C++ Compiler**: Requires a compiler that supports **C++20** (e.g., GCC 10+, Clang 10+, MSVC 19.29+).

## Building

You can compile the project using `g++`:

```bash
cd src
g++ main.cpp -o bpe -std=c++20
```

## Usage

### File Structure

- `src/`: Source code.
- `data/`: Directory for input text files (e.g., `test.txt` for training, `test2.txt` for encoding).
- `model/`: Generated artifacts (`vocab.json`, `merges.txt`) will be saved here.

### Example

The `src/main.cpp` file demonstrates how to use the `Encoding` class:

```cpp
#include <iostream>
#include "./include/BPE.hpp"

using namespace std;

int main()
{
    Tokenization tk;
    
    //Encoding
    
    vector<long long> token;
    vector<string> pair;
    
    tk.fit("../data/test.txt", 100);
    tk.encoding("../data/test2.txt", pair, token);

    for (int i = 0; i < pair.size(); ++i) cout << token[i] << " ";
    cout << endl << endl;
    
    //Decoding
    
    cout << tk.decoding(355) << endl << endl;
    
    pair.clear();

    tk.decoding(token, pair);

    for (int i = 0; i < pair.size(); ++i) cout << pair[i] << " ";
    cout << endl << endl;
    
    return 0;
}
```

### Running the Example

1. Ensure you have some text data in `data/test.txt` and `data/test2.txt`.
2. Compile and run:

```bash
cd src
./bpe
```

## License

This project is licensed under the Apache License 2.0.
