#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

// Enum to represent the different types of tokens
enum class TokenType {
    // Keywords
    INT,
    IF,

    // Identifiers
    IDENTIFIER,

    // Literals
    INTEGER_LITERAL,

    // Operators
    ASSIGN,         // =
    PLUS,           // +
    MINUS,          // -
    EQUAL,          // ==

    // Symbols
    LPAREN,         // (
    RPAREN,         // )
    LBRACE,         // {
    RBRACE,         // }
    SEMICOLON,      // ;

    // Special Tokens
    END_OF_FILE,
    UNKNOWN
};

// Structure to hold token information
struct Token {
    TokenType type;
    std::string value;
};

// Lexer class declaration
class Lexer {
public:
    Lexer(const std::string& source);
    Token getNextToken();

private:
    std::string source;
    size_t position;

    char peek();
    char advance();
    void skipWhitespace();
};

#endif // LEXER_H
