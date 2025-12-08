#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

enum class TokenType {
    INT,
    IF,
    IDENTIFIER,
    INTEGER_LITERAL,
    ASSIGN,
    PLUS,
    MINUS,
    EQUAL,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    SEMICOLON,
    END_OF_FILE,
    UNKNOWN
};

struct Token
{   
    TokenType type;
    std::string value;
};

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