#pragma once

#include "minsh/tokenizer.hpp"

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <functional>
#include <map>
#include <unistd.h>

namespace minsh {
    using BuiltinFunc = void(const std::vector<Token>& argv);

    bool is_builtin(const std::string& name);
    int run_builtin(const std::vector<Token>& argv);
    void run_clear([[maybe_unused]] const std::vector<Token>& argv);
    void run_pwd([[maybe_unused]] const std::vector<Token>& argv);
    void run_cd(const std::vector<Token>& argv);

    const std::map<std::string, std::function<BuiltinFunc>> builtins = {
        {"pwd", &run_pwd},
        {"cd", &run_cd},
        {"clear", &run_clear}
        //"exit",
        //"export",
    };
}