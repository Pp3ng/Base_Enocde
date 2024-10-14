#ifndef BASE16_H
#define BASE16_H

#include <string>
#include <array>
#include <stdexcept>
#include <cstdint>

namespace base16
{
    constexpr std::array<char, 16> encoding_table{{'0', '1', '2', '3', '4', '5', '6', '7',
                                                   '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'}};

    constexpr std::array<unsigned char, 256> create_decoding_table() noexcept
    {
        std::array<unsigned char, 256> dt{};
        for (int i = 0; i < 256; ++i)
        {
            dt[i] = 0xFF;
        }
        for (int i = 0; i < 16; i++)
        {
            dt[static_cast<unsigned char>(encoding_table[i])] = i;
        }
        return dt;
    }

    constexpr std::array<unsigned char, 256> decoding_table = create_decoding_table();

    constexpr bool is_base16(unsigned char c)
    {
        return decoding_table[c] != 0xFF;
    }

    std::string encode(const std::string &input)
    {
        std::string output;
        output.reserve(input.size() * 2);

        for (unsigned char c : input)
        {
            output.push_back(encoding_table[c >> 4]);
            output.push_back(encoding_table[c & 0x0F]);
        }

        return output;
    }

    std::string decode(const std::string &input)
    {
        if (input.empty() || input.size() % 2 != 0)
        {
            throw std::invalid_argument("无效的base16字符串长度");
        }

        std::string output;
        output.reserve(input.size() / 2);

        for (size_t i = 0; i < input.size(); i += 2)
        {
            unsigned char high = static_cast<unsigned char>(input[i]);
            unsigned char low = static_cast<unsigned char>(input[i + 1]);

            if (!is_base16(high) || !is_base16(low))
            {
                throw std::invalid_argument("base16字符串中存在无效字符");
            }

            output.push_back((decoding_table[high] << 4) | decoding_table[low]);
        }

        return output;
    }

} // namespace base16

#endif // BASE16_H
