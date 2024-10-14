#include <string>
#include <iostream>
#include <cassert>
#include "Base64.hpp"
#include "Base32.hpp"
#include "Base16.hpp"

const std::string input = "你好, Hello, 안년하세요, こんにちは, привет, Hola, Bonjour, Hallo, Ciao, Olá, здравствуйте, Namaste, مرحبا, สวัสดี, Chào bạn";

void test_encoding(const std::string &input, const std::string &encoding_name,
                   std::string (*encode_func)(const std::string &),
                   std::string (*decode_func)(const std::string &))
{
    std::string encoded = encode_func(input);
    std::string decoded = decode_func(encoded);
    if (input == decoded)
    {
        std::cout << encoding_name << " test passed" << std::endl;
    }
    else
    {
        std::cout << encoding_name << " test failed" << std::endl;
        std::cout << "Encoded: " << encoded << std::endl;
        std::cout << "Decoded: " << decoded << std::endl;
    }
}

int main(void)
{
    test_encoding(input, "Base64", base64::encode, base64::decode);
    test_encoding(input, "Base32", base32::encode, base32::decode);
    test_encoding(input, "Base16", base16::encode, base16::decode);
    return 0;
}