#include "minsh/builtins.hpp"

namespace minsh {
    bool is_builtin(const std::string& name) {
        return builtins.find(name) != builtins.end();
    }

    int run_builtin(const std::vector<Token>& argv) {
        const std::string& command_name = argv[0].value;

        auto it_found = builtins.find(command_name);

        if (it_found != builtins.end())
            return it_found->second(argv);

        return 0;
    }

    int run_clear([[maybe_unused]] const std::vector<Token>& argv) {
        std::cout << "\033[2J\033[H" << std::flush;

        return 0;
    }

    int run_cd(const std::vector<Token>& argv) {
        std::string path;
        char* home_dir = getenv("HOME");

        if (argv.size() == 1) 
            path = home_dir;
        else {
            size_t pos = argv[1].value.find("~");

            path = argv[1].value;

            if (pos != std::string::npos)
                path.replace(pos, 1, home_dir);
        }

        if (chdir(path.c_str()) != 0) {
            perror("cd");
            return 1;
        }

        return 0;
    }

    static inline std::optional<std::pair<std::string, std::string>> parse_key_value_pair(std::string str) {
        size_t length = str.size();
        size_t pos = str.find("=", 0);

        if (pos == std::string::npos)
            return std::nullopt;
        
        std::string key = str.substr(0, pos);
        std::string val = str.substr(pos + 1, length - pos);

        return std::make_pair(key, val);
    } 

    int run_export(const std::vector<Token>& argv) {
        if (argv.size() == 1) {
            for (const auto& [key, val] : get_minsh_variables())
                std::cout << key << '=' << val << '\n';

            return 0;
        }

        else {
            std::string key_val = argv[1].value;
            std::optional<std::pair<std::string, std::string>> pair = parse_key_value_pair(key_val);

            if (!pair.has_value()) {
                std::cout << "minsh: bad assignment" << '\n';
                return 1;
            }

            auto [key, value] = pair.value();

            add_variable(key, value);

            return 0;
        }
    }
}