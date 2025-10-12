#include "minsh/parser.hpp"
#include "minsh/tokenizer.hpp"
#include "minsh/ast.hpp"
#include "minsh/builtins.hpp"

#include <iostream>
#include <vector>

namespace minsh {
    Parser::Parser(std::vector<Token>& tks) : tokens(tks), current_pos(0) {}

    ParseResult Parser::parse_line() {
        ParseResult result;
        Pipeline pipeline;
        Command command;

        size_t tokens_length = tokens.size();

        for (; current_pos < tokens_length; ++current_pos) {
            Token token = tokens[current_pos];

            if (token.type == TokenType::Word) {
                command.argv.push_back(token);

                if (current_pos == 0 || tokens[current_pos - 1].type == TokenType::Pipe) {
                    if (is_builtin(token.value))
                        command.is_builtin = true;
                }
            }
            else if (token.type == TokenType::Pipe) {
                if (current_pos > 0 && tokens[current_pos - 1].type != TokenType::Pipe) {
                    pipeline.stages.push_back(command);
                    command = Command{};
                } else {
                    result.error = "Invalid syntax";
                    return result;
                }
            } 
            else {
                if(!is_valid_redirection()) {
                    result.error = "Invalid syntax";
                    return result;
                }

                handle_redirection(command);
                current_pos += 1;
                pipeline.stages.push_back(command);
                command = Command{};
            }
        }

        pipeline.stages.push_back(command);
        result.pipeline = pipeline;
        return result;
    }

    void Parser::handle_redirection(Command &cmd) {
        Redirection redir;

        redir.is_redir = true;

        switch(tokens[current_pos].type) {
            case TokenType::RedirectOut:
                redir.out = tokens[current_pos + 1].value;
                break;
            case TokenType::RedirectAppend:
                redir.out = tokens[current_pos + 1].value;
                redir.append = true;
                break;
            case TokenType::RedirectIn:
                redir.in = tokens[current_pos + 1].value;
                break;
            default:
                break;  
        }

        cmd.redir = redir;
    }

    bool Parser::is_valid_redirection() {
        if (current_pos == tokens.size() - 1 || current_pos == 0)
            return false;

        return tokens[current_pos - 1].type == TokenType::Word && tokens[current_pos + 1].type == TokenType::Word;
    }
}