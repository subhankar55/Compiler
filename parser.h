#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"
#include <vector>
#include <memory>

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    std::unique_ptr<Program> parse();

private:
    std::vector<Token> tokens;
    size_t position;

    Token peek();
    Token advance();
    bool isAtEnd();
    void consume(TokenType type, const std::string& message);

    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Statement> parseVarDeclaration();
    std::unique_ptr<Statement> parseIfStatement();
    std::unique_ptr<Statement> parseAssignmentStatement(const Token& identifierToken);
    std::unique_ptr<BlockStatement> parseBlockStatement();
    
    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Expression> parsePrimary();
};

#endif // PARSER_H
