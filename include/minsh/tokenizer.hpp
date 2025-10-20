#pragma once

#include <string>
#include <vector>

namespace minsh {
    enum class TokenType {
        Word,
        Pipe,
        RedirectIn,
        RedirectOut,
        RedirectAppend,
        Variable
    };

    struct Token {
        TokenType type;
        std::string value;
    };
    
    std::vector<Token> tokenize(std::string& line);

    inline void ltrim(std::string &s);
    inline void rtrim(std::string &s);
}