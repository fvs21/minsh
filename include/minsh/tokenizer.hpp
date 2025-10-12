#pragma once

#include <string>
#include <vector>

namespace minsh {
    enum class TokenType {
        Word,
        Pipe,
        RedirectIn,
        RedirectOut,
        RedirectAppend
    };

    struct Token {
        TokenType type;
        std::string value;
    };
    
    std::vector<Token> tokenize(const std::string& line);
}