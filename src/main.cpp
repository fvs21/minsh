#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

#include "minsh/tokenizer.hpp"
#include "minsh/parser.hpp"
#include "minsh/ast.hpp"
#include "minsh/executor.hpp"

[[maybe_unused]] static void print_pipeline(const minsh::Pipeline& pipeline) {
    for (const minsh::Command& cmd : pipeline.stages) {
        for(const minsh::Token& token : cmd.argv)
            std::cout << token.value << " ";
        std::cout << '\n';
        
        if (cmd.redir.is_redir)
            std::cout << (cmd.redir.in.size() > 0 ? cmd.redir.in : cmd.redir.out) << '\n';
    }
}

int main() {
    std::string line;
    char* username = getlogin();
    char cwd[1024];

    if (username == NULL) {
        perror("login");
        exit(1);
    }

    while (true) {
        if (getcwd(cwd, sizeof(cwd)) == NULL)
            perror("error");
        
        std::cout << "\033[2;37m" << username << "\033[0m" << ' ';
        std::cout << "\033[34m" << cwd << "\033[0m" << '\n';
        std::cout << "minsh$ " << std::flush;
        
        if (!std::getline(std::cin, line)) {
            std::cout << "\nbye\n";
            break;
        }

        if (line.empty())
            continue;

        std::vector<minsh::Token> tokens = minsh::tokenize(line);

        minsh::Parser parser(tokens);
        minsh::ParseResult result = parser.parse_line();

        if (result.error.size() > 0) {
            std::cout << result.error << '\n';
            continue;
        }

        if (!result.pipeline.has_value())
            continue;

        //print_pipeline(result.pipeline.value());

        minsh::Executor executor;
        executor.execute(result.pipeline.value());
    }
}