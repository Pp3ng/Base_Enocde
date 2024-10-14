#ifndef BASE64_H
#define BASE64_H

#include <string>
#include <array>
#include <stdexcept>
#include <cstdint>

namespace base64
{
    constexpr std::array<char, 64> encoding_table{{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                                                   'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                                                   'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                                                   'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                                                   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'}};

    constexpr std::array<unsigned char, 256> create_decoding_table()
    {
        std::array<unsigned char, 256> dt{};
        for (int i = 0; i < 256; ++i)
        {
            dt[i] = 0xFF;
        }
        for (int i = 0; i < 64; i++)
        {
            dt[static_cast<unsigned char>(encoding_table[i])] = i;
        }
        return dt;
    }

    constexpr std::array<unsigned char, 256> decoding_table = create_decoding_table();

    constexpr bool is_base64(unsigned char c)
    {
        return (c == '=') || (decoding_table[c] != 0xFF);
    }

    std::string encode(const std::string &input)
    {
        std::string output;
        output.reserve(((input.size() + 2) / 3) * 4);

        for (size_t i = 0; i < input.size(); i += 3)
        {
            uint32_t octet_a = i < input.size() ? static_cast<unsigned char>(input[i]) : 0;
            uint32_t octet_b = i + 1 < input.size() ? static_cast<unsigned char>(input[i + 1]) : 0;
            uint32_t octet_c = i + 2 < input.size() ? static_cast<unsigned char>(input[i + 2]) : 0;

            uint32_t triple = (octet_a << 16) + (octet_b << 8) + octet_c;

            output.push_back(encoding_table[(triple >> 18) & 0x3F]);
            output.push_back(encoding_table[(triple >> 12) & 0x3F]);
            output.push_back(i + 1 < input.size() ? encoding_table[(triple >> 6) & 0x3F] : '=');
            output.push_back(i + 2 < input.size() ? encoding_table[triple & 0x3F] : '=');
        }

        return output;
    }

    std::string decode(const std::string &input)
    {
        if (input.empty() || input.size() % 4 != 0)
        {
            throw std::invalid_argument("Invalid base64 string length");
        }

        size_t padding = 0;
        if (input.size() > 0 && input[input.size() - 1] == '=')
            padding++;
        if (input.size() > 1 && input[input.size() - 2] == '=')
            padding++;

        std::string output;
        output.reserve(((input.size() / 4) * 3) - padding);

        for (size_t i = 0; i < input.size(); i += 4)
        {
            uint32_t sextet_a = decoding_table[static_cast<unsigned char>(input[i])];
            uint32_t sextet_b = decoding_table[static_cast<unsigned char>(input[i + 1])];
            uint32_t sextet_c = decoding_table[static_cast<unsigned char>(input[i + 2])];
            uint32_t sextet_d = decoding_table[static_cast<unsigned char>(input[i + 3])];

            if (sextet_a == 0xFF || sextet_b == 0xFF ||
                (input[i + 2] != '=' && sextet_c == 0xFF) ||
                (input[i + 3] != '=' && sextet_d == 0xFF))
            {
                throw std::invalid_argument("Invalid character in base64 string");
            }

            uint32_t triple = (sextet_a << 18) + (sextet_b << 12) +
                              (sextet_c << 6) + sextet_d;

            output.push_back(static_cast<char>((triple >> 16) & 0xFF));
            if (input[i + 2] != '=')
                output.push_back(static_cast<char>((triple >> 8) & 0xFF));
            if (input[i + 3] != '=')
                output.push_back(static_cast<char>(triple & 0xFF));
        }

        return output;
    }

} // namespace base64

#endif // BASE64_H