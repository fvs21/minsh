#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "minsh/tokenizer.hpp"
#include "minsh/parser.hpp"
#include "minsh/ast.hpp"
#include "minsh/executor.hpp"
#include "minsh/variables.hpp"

[[maybe_unused]] static void print_pipeline(const minsh::Pipeline& pipeline) {
    for (const minsh::Command& cmd : pipeline.stages) {
        for (const minsh::Token& token : cmd.argv)
            std::cout << token.value << " ";
        std::cout << '\n';
        
        if (cmd.redir.is_redir)
            std::cout << (cmd.redir.in.size() > 0 ? cmd.redir.in : cmd.redir.out) << '\n';
    }
}

static std::string get_prompt(std::string username, char cwd[1024]) {
    std::string cwd_str(cwd);

    std::string prompt = "\033[2;37m" + username + "\033[0m ";
    prompt.append("\033[34m" + cwd_str + "\033[0m\n");
    prompt.append("minsh$ ");

    return prompt;
}

[[maybe_unused]] static void print_vector(const std::vector<minsh::Token>& vec) {
    std::cout << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i].value;
        if (i < vec.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

int main() {
    std::string line;
    char* username = getlogin();
    char cwd[1024];

    rl_bind_key('\t', rl_complete);

    if (username == NULL) {
        perror("login");
        exit(1);
    }

    minsh::add_variable("USER", username);

    while (true) {
        if (getcwd(cwd, sizeof(cwd)) == NULL)
            perror("error");

        std::string prompt = get_prompt(username, cwd);
        line = readline(prompt.c_str());
        
        if (!line.c_str()) {
            std::cout << "\nbye\n";
            break;
        }

        if (line.empty())
            continue;

        add_history(line.c_str());

        std::vector<minsh::Token> tokens = minsh::tokenize(line);

        minsh::Parser parser(tokens);
        minsh::ParseResult result = parser.parse_line();

        if (result.error.size() > 0) {
            std::cout << result.error << '\n';
            continue;
        }

        if (!result.pipeline.has_value())
            continue;

        minsh::Executor executor;
        executor.execute(result.pipeline.value());
    }
}