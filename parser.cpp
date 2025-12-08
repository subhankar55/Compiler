#include "parser.h"
#include <iostream>
#include <stdexcept>


Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), position(0) {}


Token Parser::peek() {
    if (isAtEnd()) return {TokenType::END_OF_FILE, ""};
    return tokens[position];
}


Token Parser::advance() {
    if (!isAtEnd()) position++;
    return tokens[position - 1];
}


bool Parser::isAtEnd() {
    return position >= tokens.size() || tokens[position].type == TokenType::END_OF_FILE;
}


void Parser::consume(TokenType type, const std::string& message) {
    if (peek().type == type) {
        advance();
        return;
    }
    throw std::runtime_error("Parser Error: " + message);
}

std::unique_ptr<Program> Parser::parse() {
    auto program = std::make_unique<Program>();
    while (!isAtEnd()) {
        program->statements.push_back(parseStatement());
    }
    return program;
}


std::unique_ptr<Statement> Parser::parseStatement() {
    if (peek().type == TokenType::INT) {
        return parseVarDeclaration();
    }
    if (peek().type == TokenType::IF) {
        return parseIfStatement();
    }
    if (peek().type == TokenType::IDENTIFIER) {
        
        if (position + 1 < tokens.size() && tokens[position + 1].type == TokenType::ASSIGN) {
             return parseAssignmentStatement(peek());
        }
    }
   
    throw std::runtime_error("Parser Error: Unexpected token " + peek().value);
}


std::unique_ptr<Statement> Parser::parseVarDeclaration() {
    consume(TokenType::INT, "Expected 'int' keyword.");
    Token identifier = peek();
    consume(TokenType::IDENTIFIER, "Expected identifier after 'int'.");
    consume(TokenType::SEMICOLON, "Expected ';' after variable declaration.");
    return std::make_unique<VarDecl>(identifier.value);
}


std::unique_ptr<Statement> Parser::parseAssignmentStatement(const Token& identifierToken) {
    consume(TokenType::IDENTIFIER, "Expected an identifier for assignment.");
    consume(TokenType::ASSIGN, "Expected '=' for assignment.");
    auto value = parseExpression();
    consume(TokenType::SEMICOLON, "Expected ';' after assignment.");
    return std::make_unique<Assignment>(identifierToken.value, std::move(value));
}


std::unique_ptr<Statement> Parser::parseIfStatement() {
    consume(TokenType::IF, "Expected 'if' keyword.");
    consume(TokenType::LPAREN, "Expected '(' after 'if'.");
    auto condition = parseExpression();
    consume(TokenType::RPAREN, "Expected ')' after if condition.");
    auto body = parseBlockStatement();
    return std::make_unique<IfStatement>(std::move(condition), std::move(body));
}


std::unique_ptr<BlockStatement> Parser::parseBlockStatement() {
    auto block = std::make_unique<BlockStatement>();
    consume(TokenType::LBRACE, "Expected '{' to start a block.");
    while (peek().type != TokenType::RBRACE && !isAtEnd()) {
        block->statements.push_back(parseStatement());
    }
    consume(TokenType::RBRACE, "Expected '}' to end a block.");
    return block;
}





std::unique_ptr<Expression> Parser::parseExpression() {
    auto left = parsePrimary();


    while (peek().type == TokenType::PLUS || peek().type == TokenType::MINUS || peek().type == TokenType::EQUAL) {
        Token op = advance();
        auto right = parsePrimary();
        left = std::make_unique<BinaryOp>(op.value, std::move(left), std::move(right));
    }


    return left;
}


std::unique_ptr<Expression> Parser::parsePrimary() {
    if (peek().type == TokenType::INTEGER_LITERAL) {
        int value = std::stoi(advance().value);
        return std::make_unique<NumberLiteral>(value);
    }
    if (peek().type == TokenType::IDENTIFIER) {
        std::string name = advance().value;
        return std::make_unique<Identifier>(name);
    }
   
    throw std::runtime_error("Parser Error: Unexpected expression " + peek().value);
}
