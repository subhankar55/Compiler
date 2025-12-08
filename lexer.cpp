#include "lexer.h"
#include <cctype>
#include <iostream>


Lexer::Lexer(const std::string& source) : source(source), position(0) {}


char Lexer::peek() {
    if (position >= source.length()) {
        return '\0';
    }
    return source[position];
}


char Lexer::advance() {
    if (position >= source.length()) {
        return '\0';
    }
    return source[position++];
}


void Lexer::skipWhitespace() {
    while (peek() != '\0' && isspace(peek())) {
        advance();
    }
}


Token Lexer::getNextToken() {
    skipWhitespace();


    char current_char = peek();

    if (current_char == '\0') {
        return {TokenType::END_OF_FILE, ""};
    }


    if (isdigit(current_char)) {
        std::string number;
        while (peek() != '\0' && isdigit(peek())) {
            number += advance();
        }
        return {TokenType::INTEGER_LITERAL, number};
    }


    if (isalpha(current_char) || current_char == '_') {
        std::string identifier;
        while (peek() != '\0' && (isalnum(peek()) || peek() == '_')) {
            identifier += advance();
        }
        if (identifier == "int") return {TokenType::INT, identifier};
        if (identifier == "if") return {TokenType::IF, identifier};
        return {TokenType::IDENTIFIER, identifier};
    }


    switch (current_char) {
        case '=':
            advance();
            if (peek() == '=') {
                advance();
                return {TokenType::EQUAL, "=="};
            }
            return {TokenType::ASSIGN, "="};
        case '+':
            advance();
            return {TokenType::PLUS, "+"};
        case '-':
            advance();
            return {TokenType::MINUS, "-"};
        case '(':
            advance();
            return {TokenType::LPAREN, "("};
        case ')':
            advance();
            return {TokenType::RPAREN, ")"};
        case '{':
            advance();
            return {TokenType::LBRACE, "{"};
        case '}':
            advance();
            return {TokenType::RBRACE, "}"};
        case ';':
            advance();
            return {TokenType::SEMICOLON, ";"};
    }


    advance();
    return {TokenType::UNKNOWN, std::string(1, current_char)};
}
