#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "CodeGenerator.h"
#include "CPU.h"


std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::INT: return "INT";
        case TokenType::IF: return "IF";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::INTEGER_LITERAL: return "INTEGER_LITERAL";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::LBRACE: return "LBRACE";
        case TokenType::RBRACE: return "RBRACE";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::END_OF_FILE: return "END_OF_FILE";
        case TokenType::UNKNOWN: return "UNKNOWN";
        default: return "INVALID_TOKEN_TYPE";
    }
}


void printAST(const Node* node, int indent = 0) {
    if (!node) return;
    std::string indentation(indent * 2, ' ');


    if (auto p = dynamic_cast<const Program*>(node)) {
        std::cout << indentation << "Program" << std::endl;
        for (const auto& stmt : p->statements) {
            printAST(stmt.get(), indent + 1);
        }
    } else if (auto vd = dynamic_cast<const VarDecl*>(node)) {
        std::cout << indentation << "VarDecl: " << vd->varName << std::endl;
    } else if (auto a = dynamic_cast<const Assignment*>(node)) {
        std::cout << indentation << "Assignment: " << a->varName << std::endl;
        printAST(a->value.get(), indent + 1);
    } else if (auto is = dynamic_cast<const IfStatement*>(node)) {
        std::cout << indentation << "IfStatement" << std::endl;
        std::cout << indentation << "  Condition:" << std::endl;
        printAST(is->condition.get(), indent + 2);
        std::cout << indentation << "  Body:" << std::endl;
        printAST(is->body.get(), indent + 2);
    } else if (auto bs = dynamic_cast<const BlockStatement*>(node)) {
        std::cout << indentation << "Block" << std::endl;
        for (const auto& stmt : bs->statements) {
            printAST(stmt.get(), indent + 1);
        }
    } else if (auto bo = dynamic_cast<const BinaryOp*>(node)) {
        std::cout << indentation << "BinaryOp: " << bo->op << std::endl;
        printAST(bo->left.get(), indent + 1);
        printAST(bo->right.get(), indent + 1);
    } else if (auto nl = dynamic_cast<const NumberLiteral*>(node)) {
        std::cout << indentation << "Number: " << nl->value << std::endl;
    } else if (auto id = dynamic_cast<const Identifier*>(node)) {
        std::cout << indentation << "Identifier: " << id->name << std::endl;
    }
}


int main() {
    std::string source_code = R"(
        // Variable declaration
        int a;
        int b;
        int c;
        // Assignment
        a = 10;
        b = 20;
        c = a + b;
        // Conditional
        if (c == 30) {
            c = c + 1;
        }
    )";
   
    size_t pos;
    while ((pos = source_code.find("//")) != std::string::npos) {
        size_t end_of_line = source_code.find('\n', pos);
        source_code.erase(pos, end_of_line - pos);
    }


    Lexer lexer(source_code);
    std::vector<Token> tokens;
    Token token;
    do {
        token = lexer.getNextToken();
        if (token.type != TokenType::UNKNOWN) { 
            tokens.push_back(token);
        }
    } while (token.type != TokenType::END_OF_FILE);


    std::cout << "--- Lexer Output ---" << std::endl;
    for(const auto& t : tokens){
         std::cout << "Type: " << tokenTypeToString(t.type)
                  << ", Value: '" << t.value << "'" << std::endl;
    }


    std::unique_ptr<Program> ast;
    try {
        Parser parser(tokens);
        ast = parser.parse();
       
        std::cout << "\n--- AST Output ---" << std::endl;
        printAST(ast.get());
        std::cout << "\nParsing completed successfully." << std::endl;


    } catch (const std::exception& e) {
        std::cerr << "\n" << e.what() << std::endl;
        return 1; 
    }

    std::string assembly;
    if (ast) { 
        try {
            CodeGenerator generator;
            assembly = generator.generate(*ast);


            std::cout << "\n--- Assembly Output ---" << std::endl;
            std::cout << assembly << std::endl;
            std::cout << "Code generation completed successfully." << std::endl;


        } catch (const std::exception& e) {
            std::cerr << "\n" << e.what() << std::endl;
            return 1; 
        }
    }


    if (!assembly.empty()) {
        try {
            CPU cpu;
            cpu.loadProgram(assembly);
            cpu.run();
           
            std::cout << "\n--- Simulation Results ---" << std::endl;
            cpu.printState();
            cpu.printMemory(0, 3);
            std::cout << "\nIntegration test completed successfully." << std::endl;


        } catch (const std::exception& e) {
            std::cerr << "\nCPU Simulation Error: " << e.what() << std::endl;
            return 1;
        }
    }


    return 0;
}
