#include "minsh/variables.hpp" 

#include "stdlib.h"

namespace minsh {
    std::map<std::string, std::string>& get_minsh_variables() {
        static std::map<std::string, std::string> minsh_vars;
        return minsh_vars;
    }

    std::string get_variable_value(std::string variable_name) {
        auto& vars = get_minsh_variables();

        if (vars.count(variable_name))
            return vars[variable_name];

        return "";
    }

    void add_variable(std::string name, std::string value) {
        get_minsh_variables()[name] = value;

        setenv(name.c_str(), value.c_str(), 1);
    }
}