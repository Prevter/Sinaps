#include <sinaps/lexer.hpp>
#include <sstream>

namespace sinaps::lexer {

    bool is_whitespace(char c) {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
    }

    bool is_digit(char c) {
        return c >= '0' && c <= '9';
    }

    bool is_hex(char c) {
        return is_digit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
    }

    bool eat_token(const std::string &str, size_t &index, char token) {
        if (index < str.size() && str[index] == token) {
            index++;
            return true;
        }
        return false;
    }

    SINAPS_API Result<std::vector<pattern_token>> lexPattern(const std::string &pattern) {
        std::vector<pattern_token> tokens;
        size_t index = 0;
        while (index < pattern.size()) {
            if (is_whitespace(pattern[index])) {
                index++;
                continue;
            }
            if (eat_token(pattern, index, '^')) {
                tokens.push_back({pattern_token_type::NOT, 0});
                continue;
            }
            if (eat_token(pattern, index, '(')) {
                tokens.push_back({pattern_token_type::L_PAREN, 0});
                continue;
            }
            if (eat_token(pattern, index, ')')) {
                tokens.push_back({pattern_token_type::R_PAREN, 0});
                continue;
            }
            if (eat_token(pattern, index, '[')) {
                tokens.push_back({pattern_token_type::L_BRACKET, 0});
                continue;
            }
            if (eat_token(pattern, index, ']')) {
                tokens.push_back({pattern_token_type::R_BRACKET, 0});
                continue;
            }
            if (eat_token(pattern, index, '|')) {
                tokens.push_back({pattern_token_type::OR, 0});
                continue;
            }
            if (eat_token(pattern, index, '*')) {
                tokens.push_back({pattern_token_type::STAR, 0});
                continue;
            }
            if (eat_token(pattern, index, '$')) {
                tokens.push_back({pattern_token_type::DOLLAR, 0});
                continue;
            }
            if (eat_token(pattern, index, '+')) {
                tokens.push_back({pattern_token_type::PLUS, 0});
                continue;
            }
            if (eat_token(pattern, index, '-')) {
                tokens.push_back({pattern_token_type::MINUS, 0});
                continue;
            }
            if (eat_token(pattern, index, '?')) {
                tokens.push_back({pattern_token_type::QUESTION, 0});
                continue;
            }

            // Check if two characters are a hex value
            bool has_two_chars = index + 1 < pattern.size();
            if (has_two_chars && is_hex(pattern[index]) && is_hex(pattern[index + 1])) {
                uint8_t value = 0;
                if (is_digit(pattern[index])) {
                    value = (pattern[index] - '0') << 4;
                } else if (pattern[index] >= 'A' && pattern[index] <= 'F') {
                    value = (pattern[index] - 'A' + 10) << 4;
                } else if (pattern[index] >= 'a' && pattern[index] <= 'f') {
                    value = (pattern[index] - 'a' + 10) << 4;
                }
                if (is_digit(pattern[index + 1])) {
                    value |= pattern[index + 1] - '0';
                } else if (pattern[index + 1] >= 'A' && pattern[index + 1] <= 'F') {
                    value |= pattern[index + 1] - 'A' + 10;
                } else if (pattern[index + 1] >= 'a' && pattern[index + 1] <= 'f') {
                    value |= pattern[index + 1] - 'a' + 10;
                }
                tokens.push_back({pattern_token_type::BYTE, value});
                index += 2;
                continue;
            }

            // Unknown token
            std::stringstream ss;
            ss << "Unknown token at index " << index << ": " << pattern[index];
            return Result<std::vector<pattern_token>>::fail(ss.str());
        }

        return Result<std::vector<pattern_token>>::ok(tokens);
    }

}