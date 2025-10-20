#pragma once

#include "minsh/tokenizer.hpp"
#include "minsh/variables.hpp"

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <functional>
#include <map>
#include <utility>
#include <optional>
#include <unistd.h>

namespace minsh {
    using BuiltinFunc = int(const std::vector<Token>& argv);

    bool is_builtin(const std::string& name);
    int run_builtin(const std::vector<Token>& argv);
    int run_clear([[maybe_unused]] const std::vector<Token>& argv);
    int run_cd(const std::vector<Token>& argv);
    int run_export(const std::vector<Token>& argv);

    const std::map<std::string, std::function<BuiltinFunc>> builtins = {
        {"cd", &run_cd},
        {"clear", &run_clear},
        {"export", &run_export}
        //"exit",
    };
}