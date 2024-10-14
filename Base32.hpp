#ifndef BASE32_H
#define BASE32_H

#include <string>
#include <array>
#include <stdexcept>
#include <cstdint>

namespace base32
{
    constexpr std::array<char, 32> encoding_table{{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                                   'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                                   'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                                   'Y', 'Z', '2', '3', '4', '5', '6', '7'}};

    constexpr std::array<unsigned char, 256> create_decoding_table() noexcept
    {
        std::array<unsigned char, 256> dt{};
        for (int i = 0; i < 256; ++i)
        {
            dt[i] = 0xFF;
        }
        for (int i = 0; i < 32; i++)
        {
            dt[static_cast<unsigned char>(encoding_table[i])] = i;
        }
        return dt;
    }

    constexpr std::array<unsigned char, 256> decoding_table = create_decoding_table();

    constexpr bool is_base32(unsigned char c)
    {
        return (c == '=') || (decoding_table[c] != 0xFF);
    }

    std::string encode(const std::string &input)
    {
        std::string output;
        output.reserve(((input.size() + 4) / 5) * 8);

        for (size_t i = 0; i < input.size(); i += 5)
        {
            uint64_t buffer = 0;
            int bits_in_buffer = 0;
            for (int j = 0; j < 5 && i + j < input.size(); ++j)
            {
                buffer = (buffer << 8) | static_cast<unsigned char>(input[i + j]);
                bits_in_buffer += 8;
            }

            while (bits_in_buffer > 0)
            {
                if (bits_in_buffer >= 5)
                {
                    output.push_back(encoding_table[(buffer >> (bits_in_buffer - 5)) & 0x1F]);
                    bits_in_buffer -= 5;
                }
                else
                {
                    buffer <<= (5 - bits_in_buffer);
                    output.push_back(encoding_table[buffer & 0x1F]);
                    bits_in_buffer = 0;
                }
            }
        }

        // Padding
        while (output.size() % 8 != 0)
        {
            output.push_back('=');
        }

        return output;
    }

    std::string decode(const std::string &input)
    {
        if (input.empty() || input.size() % 8 != 0)
        {
            throw std::invalid_argument("Invalid base32 string length");
        }

        std::string output;
        output.reserve((input.size() / 8) * 5);

        for (size_t i = 0; i < input.size(); i += 8)
        {
            uint64_t buffer = 0;
            int bits_in_buffer = 0;

            for (int j = 0; j < 8 && input[i + j] != '='; ++j)
            {
                unsigned char c = static_cast<unsigned char>(input[i + j]);
                if (!is_base32(c))
                {
                    throw std::invalid_argument("Invalid character in base32 string");
                }
                buffer = (buffer << 5) | decoding_table[c];
                bits_in_buffer += 5;
            }

            while (bits_in_buffer >= 8)
            {
                bits_in_buffer -= 8;
                output.push_back(static_cast<char>((buffer >> bits_in_buffer) & 0xFF));
            }
        }

        return output;
    }

} // namespace base32

#endif // BASE32_H