# Compiler

SimpleLang Compiler and 8-bit CPU Simulator
— Detailed Documentation
This document provides a fully expanded explanation of the SimpleLang compiler project, covering
all phases—Lexing, Parsing, AST Generation, Code Generation, and CPU Simulation. It includes
diagrams, structured sections, and code listings.
1. System Architecture Overview
The system consists of four major components:
1. **Lexer** – Converts raw source code into tokens 2. **Parser + AST Builder** – Converts tokens
into a structured Abstract Syntax Tree 3. **Code Generator** – Translates AST into 8-bit CPU
assembly 4. **CPU Simulator** – Executes the generated assembly instructions
Overall Compiler Pipeline
Lexer Parser + AST Code Gen CPU Sim
2. Lexer (Tokenization)
The lexer scans the raw input source code and produces a sequence of tokens. It handles:
• Keywords: `int`, `if` • Identifiers • Integer literals • Operators: `=`, `+`, `-`, `==` • Symbols: `(`, `)`, `{`,
`}`, `;`
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
3. Parser
The parser reads the token stream and constructs an Abstract Syntax Tree (AST). It supports
variable declarations, assignments, if-statements, block statements, and expressions.
std::unique_ptr<Program> Parser::parse() {
 auto program = std::make_unique<Program>();
 while (!isAtEnd()) {
 program->statements.push_back(parseStatement());
 }
 return program;
}
4. Abstract Syntax Tree (AST)
The AST uses a node-based hierarchy to represent the source program structure.
struct NumberLiteral : public Expression { int value; };
struct Identifier : public Expression { std::string name; };
struct BinaryOp : public Expression { std::string op; ... };
struct VarDecl : public Statement { std::string varName; };
struct Assignment : public Statement { ... };
struct IfStatement : public Statement { ... };
struct Program : public Node { std::vector<std::unique_ptr<Statement>> statements; };
5. Code Generator
The code generator emits assembly instructions for a simplified virtual 8-bit CPU.
assembly_code << "ldi A " << expr->value << "\n";
6. CPU Simulator
The CPU simulation executes the produced instructions, maintaining registers, memory, stack, and
flags.
void CPU::execute(const Instruction& instr) { ... }
7. Example Program
int a;
int b;
int c;
a = 10;
b = 20;
c = a + b;
if (c == 30) {
 c = c + 1;
}
Expected output: variable `c` ends up storing **31**.
8. Summary
This project showcases how real compilers work by including full stages: lexical analysis, parsing,
AST generation, code generation, and CPU-level execution.
