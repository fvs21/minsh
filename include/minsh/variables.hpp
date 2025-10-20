#pragma once

#include <map>

namespace minsh {
    std::map<std::string, std::string>& get_minsh_variables();

    std::string get_variable_value(std::string variable_name);

    void add_variable(std::string name, std::string value);
}