#include "minsh/builtins.hpp"

namespace minsh {
    bool is_builtin(const std::string& name) {
        return builtins.find(name) != builtins.end();
    }

    int run_builtin(const std::vector<Token>& argv) {
        const std::string& command_name = argv[0].value;

        auto it_found = builtins.find(command_name);

        if (it_found != builtins.end())
            it_found->second(argv);

        return 0;
    }

    void run_clear([[maybe_unused]] const std::vector<Token>& argv) {
        std::cout << "\033[2J\033[H" << std::flush;
    }

    void run_pwd([[maybe_unused]] const std::vector<Token>& argv) {
        char buffer[1024];

        if (getcwd(buffer, sizeof(buffer)) == NULL) {
            perror("getcwd");
            return;
        }

        std::cout << buffer << '\n';
    }

    void run_cd(const std::vector<Token>& argv) {
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

        if (chdir(path.c_str()) != 0)
            perror("cd");
    }
}