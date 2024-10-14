# Multiple Base Encoding Implementations

This project provides C++ implementations of Base64, Base32, and Base16 encodings. These encoding methods are commonly used to represent binary data in ASCII character sets, and are used in various applications such as encoding email attachments, URLs, and cryptographic keys.

## Features

- Implementations of Base64, Base32, and Base16 encoding and decoding
- Utilizes modern C++ features such as constexpr and std::array
- Includes a simple test program to verify encoding/decoding correctness
- Supports processing input strings containing characters from multiple languages

## File Structure

- `Base64.hpp`: Implementation of Base64 encoding
- `Base32.hpp`: Implementation of Base32 encoding
- `Base16.hpp`: Implementation of Base16 encoding
- `main.cpp`: Test program

## Usage

1. Include the header files in your project:

```cpp
#include "Base64.hpp"
#include "Base32.hpp"
#include "Base16.hpp"
```

2. Use the respective namespaces and functions for encoding or decoding:

```cpp
std::string encoded = base64::encode(input);
std::string decoded = base64::decode(encoded);
// Similarly use base32 and base16 namespaces
```

## Test

Compile and run the test program using the following commands:

```bash
g++ main.cpp -o test
./test
```

## Notes

1. Ensure that input strings are valid for Base32 and Base16 encoding
2. Base64 and Base32 encodings may add padding characters ('=') at the end of the output
