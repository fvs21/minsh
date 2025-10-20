#include "minsh/executor.hpp"
#include "minsh/builtins.hpp"
#include "minsh/tokenizer.hpp"  

#include <unistd.h>
#include <fcntl.h>
#include <iostream>

namespace minsh {
    void Executor::execute(const Pipeline& pipeline) {
        size_t num_stages = pipeline.stages.size();

        int pipe_fds[2];
        int input_fd = STDIN_FILENO;
        int output_fd;

        for (size_t i = 0; i < num_stages; ++i) {
            Command cmd = pipeline.stages[i];

            if (cmd.is_builtin) {
                run_builtin(cmd.argv);
                continue;
            }

            if (pipe(pipe_fds) < 0) {
                perror("pipe");
                exit(1);
            }

            if (i == num_stages - 1)
                output_fd = STDOUT_FILENO;
            else
                output_fd = pipe_fds[1];

            int fds[] = { input_fd, output_fd };
            
            execute_command(cmd, fds);

            close(pipe_fds[1]);
            if (i != STDIN_FILENO) close(input_fd);

            input_fd = pipe_fds[0];
        }

        if (input_fd != STDIN_FILENO) close(input_fd);
    }

    void Executor::execute_command(const Command& cmd, int fds[2]) {        
        pid_t process_id = fork();

        if (process_id < 0) {
            perror("fork");
            return;
        }

        if (process_id == 0) {
            handle_pipes(fds);

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

    void Executor::handle_pipes(int fds[2]) {
        if (fds[0] != 0) {
            dup2(fds[0], STDIN_FILENO);
            close(fds[0]);
        }

        if (fds[1] != 1) {
            dup2(fds[1], STDOUT_FILENO);
            close(fds[1]);
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
}