#pragma once

#include "minsh/ast.hpp"
#include "minsh/tokenizer.hpp"
#include <optional>
#include <string>

namespace minsh {
    struct ParseResult {
        std::optional<Pipeline> pipeline;
        std::string error;
    };

    class Parser {
        public:
            ParseResult parse_line();
            explicit Parser(std::vector<Token>& tokens);

        private:
            std::vector<Token> tokens;

            //current index in the token vector
            //usefull to check errors in prompt structure
            size_t current_pos = 0; 

            bool is_valid_redirection();
            void handle_redirection(Command &cmd); 
    };
}