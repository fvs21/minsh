#include "minsh/tokenizer.hpp"

#include <vector>
#include <algorithm>

namespace minsh {
    inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }

    inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    static TokenType parse_token_type(const std::string& token) {
        if (token.compare("|") == 0)
            return TokenType::Pipe;
        else if (token.compare(">") == 0)
            return TokenType::RedirectOut;
        else if (token.compare(">>") == 0)
            return TokenType::RedirectAppend;
        else if (token.compare("<") == 0)
            return TokenType::RedirectIn;
        
        return TokenType::Word;
    }

    std::vector<Token> tokenize(std::string& line) {
        std::vector<Token> tokens;
        const std::string delimeter = " ";

        ltrim(line);
        rtrim(line);
        
        size_t start_pos = 0, end_pos;
        
        while ((end_pos = line.find(delimeter, start_pos)) != std::string::npos) {
            std::string token = line.substr(start_pos, end_pos - start_pos);
            start_pos = end_pos += 1;

            Token token_struct = { parse_token_type(token), token };
            tokens.push_back(token_struct);
        }
        
        std::string token = line.substr(start_pos);
        Token token_struct = { parse_token_type(token), token };
        tokens.push_back(token_struct);
        
        return tokens;
    }
}