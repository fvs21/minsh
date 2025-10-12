#include "minsh/executor.hpp"
#include "minsh/builtins.hpp"
#include "minsh/tokenizer.hpp"  

#include <unistd.h>
#include <fcntl.h>
#include <iostream>


namespace minsh {
    void Executor::execute(const Pipeline& pipeline) {
        for (size_t i = 0; i < pipeline.stages.size(); ++i) {
            Command cmd = pipeline.stages[i];
            
            if (cmd.is_builtin)
                execute_builtin(cmd);
            else
                execute_command(cmd);
        }
    }

    void Executor::execute_command(const Command& cmd) {        
        pid_t process_id = fork();

        if (process_id < 0) {
            perror("fork");
            return;
        }

        if (process_id == 0) {
            if (cmd.redir.is_redir)
                handle_redirection(cmd.redir);
            
            std::vector<char*> argv;

            for (const auto& arg : cmd.argv) {
                argv.push_back(const_cast<char*>(arg.value.c_str()));
            }

            argv.push_back(nullptr);

            int status = execvp(argv[0], argv.data());

            if (status < 0) {
                perror(argv[0]);
                exit(1);
            }

        } else {
            wait(NULL);
        }
    }

    void Executor::handle_redirection(const Redirection& redir) {
        if (!redir.in.empty()) {
            int fd = open(redir.in.c_str(), O_RDONLY);

            if (fd == -1) {
                perror("open in failed");
                exit(1);
            }

            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        
        if (!redir.out.empty()) {
            int fd = open(redir.out.c_str(), O_WRONLY | O_CREAT | (redir.append ? O_APPEND : O_TRUNC), 0644);

            if (fd == -1) {
                perror("open out failed");
                exit(1);
            }

            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
    }

    void Executor::execute_builtin(const Command& cmd) {
        run_builtin(cmd.argv);
    }
}
