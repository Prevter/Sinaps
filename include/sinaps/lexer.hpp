#pragma once

#include "platform.hpp"
#include "extras.hpp"

namespace sinaps::lexer {

    // Pattern lexer
    enum class pattern_token_type {
        BYTE = 0,      // Constant byte value
        NOT = 1,       // ^ - not operator
        L_PAREN = 2,   // ( - start group
        R_PAREN = 3,   // ) - end group
        L_BRACKET = 4, // [ - start optional group
        R_BRACKET = 5, // ] - end optional group
        OR = 6,        // | - or operator
        STAR = 7,      // * - repeat operator
        QUESTION = 8,  // ? - wildcard operator
        DOLLAR = 9,    // $ - set cursor
        PLUS = 10,     // + - greater than operator
        MINUS = 11,    // - - less than operator
    };

    struct pattern_token {
        pattern_token_type type;
        uint8_t value;
    };

    SINAPS_API Result<std::vector<pattern_token>> lexPattern(const std::string &pattern);

}