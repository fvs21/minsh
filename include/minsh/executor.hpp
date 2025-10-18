#pragma once

#include "minsh/ast.hpp"

namespace minsh {
    class Executor {
        public:
            void execute(const Pipeline& pipeline);
        private:
            void execute_command(const Command& cmd, int fds[2]);
            void execute_builtin(const Command& cmd);
            void handle_redirection(const Redirection& redir);
            void handle_pipes(int fds[2]);
    };
}