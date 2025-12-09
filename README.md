SimpleLang Compiler and 8-bit CPU Simulator — Full Documentation

This document provides a fully structured and expanded explanation of the SimpleLang Compiler Project. It details all major stages of compilation:

Lexical Analysis (Lexer)

Parsing + AST Construction

Code Generation (8-bit Assembly)

CPU Simulation (Execution)

Clear diagrams, structured headings, and source code listings are included throughout.

**System Architecture Overview**

The SimpleLang compiler consists of four major modules:

Lexer – Converts raw source code into tokens

Parser + AST Builder – Converts tokens into a structured tree

Code Generator – Converts AST nodes into assembly instructions

CPU Simulator – Executes the generated assembly program step-by-step

Compiler Pipeline Overview
┌────────────┐     ┌─────────────┐     ┌──────────────┐     ┌──────────────┐
│   Source    │ --> │    Lexer     │ --> │    Parser +   │ --> │   Code Gen    │
│    Code     │     │ (Tokens)     │     │     AST       │     │ (Assembly)    │
└────────────┘     └─────────────┘     └──────────────┘     └──────────────┘
                                                             |
                                                             v
                                                     ┌──────────────┐
                                                     │   CPU Sim     │
                                                     │ (Execution)   │
                                                     └──────────────┘

**Lexer (Tokenization)**

The lexer scans raw input and produces tokens, which are the smallest meaningful units in the language.

Recognized Tokens

Keywords: int, if

Identifiers

Literals: integer numbers

Operators: =, +, -, ==

Symbols: (, ), {, }, ;

Special: END_OF_FILE, UNKNOWN

Lexer Header (lexer.h)
#ifndef LEXER_H
#define LEXER_H

enum class TokenType {
    INT, IF, IDENTIFIER, INTEGER_LITERAL,
    ASSIGN, PLUS, MINUS, EQUAL,
    LPAREN, RPAREN, LBRACE, RBRACE, SEMICOLON,
    END_OF_FILE, UNKNOWN
};

struct Token {
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

#endif

**Parser**

The parser reads the stream of tokens and constructs an Abstract Syntax Tree (AST).

It supports:

Variable declarations

Assignments

If statements

Block statements

Binary arithmetic expressions

Program Parsing Function
std::unique_ptr<Program> Parser::parse() {
    auto program = std::make_unique<Program>();

    while (!isAtEnd()) {
        program->statements.push_back(parseStatement());
    }

    return program;
}

**Abstract Syntax Tree (AST)**

The AST converts raw syntax into a structured semantic representation.

Example AST Node Definitions
struct NumberLiteral : public Expression {
    int value;
};

struct Identifier : public Expression {
    std::string name;
};

struct BinaryOp : public Expression {
    std::string op;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
};

struct VarDecl : public Statement {
    std::string varName;
};

struct Assignment : public Statement {
    std::string varName;
    std::unique_ptr<Expression> value;
};

struct IfStatement : public Statement {
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Block> thenBlock;
};

struct Program : public Node {
    std::vector<std::unique_ptr<Statement>> statements;
};

**Code Generator**

The Code Generator converts AST nodes into assembly for a simplified 8-bit CPU.

Example Code Generation
assembly_code << "ldi A " << expr->value << "\n";


Assembly type includes:

Literal loading

Register operations

Memory access

Comparison and jumps

**CPU Simulator**

The CPU simulator interprets and executes the generated assembly code.

It maintains:

Registers: A, B

Program Counter (PC)

Stack Pointer (SP)

Flags: Zero, Carry

Memory (256 bytes)

Instruction Execution Loop
void CPU::execute(const Instruction& instr) {
    // executes one instruction at a time
}

**Example Program**

Source Code:

int a;
int b;
int c;

a = 10;
b = 20;
c = a + b;

if (c == 30) {
    c = c + 1;
}

**Expected Result**
c = 31


The condition c == 30 is true, so the last block executes.

**Summary**

The SimpleLang project is a complete mini-compiler and CPU simulation system that demonstrates how real compilers work internally. It includes all major phases of compilation:

Lexical Analysis (Lexer): Breaks source code into meaningful tokens.

Parsing + AST Construction: Converts tokens into a structured Abstract Syntax Tree that represents the program.

Code Generation: Translates the AST into assembly instructions for a custom 8-bit CPU.

CPU Simulation: Executes the generated assembly using virtual registers, memory, and flags.

Together, these components form a full pipeline that takes high-level SimpleLang code and runs it on a simulated CPU. This project provides an end-to-end understanding of how programming languages are interpreted and executed—from raw source code to final machine-level behavior.
