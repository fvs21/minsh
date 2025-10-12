#pragma once

#include <string>
#include <vector>

#include "minsh/tokenizer.hpp"

namespace minsh {
    struct Redirection { 
        std::string in, out;
        bool append = false, is_redir = false;
    };

    struct Command {
        std::vector<Token> argv;
        Redirection redir;
        bool is_builtin = false;
    };

    struct Pipeline {
        std::vector<Command> stages;
    };
}