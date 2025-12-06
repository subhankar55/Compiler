#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include "ast.h"
#include <string>
#include <vector>
#include <map>
#include <sstream>

class CodeGenerator {
public:
    std::string generate(const Program& program);

private:
    std::stringstream assembly_code;
    std::map<std::string, int> variable_addresses;
    int next_address = 0;
    int label_counter = 0;

    void visit(const Statement* stmt);
    void visit(const VarDecl* stmt);
    void visit(const Assignment* stmt);
    void visit(const IfStatement* stmt);
    void visit(const BlockStatement* stmt);

    void visit(const Expression* expr);
    void visit(const NumberLiteral* expr);
    void visit(const Identifier* expr);
    void visit(const BinaryOp* expr);

    std::string newLabel();
};

#endif // CODE_GENERATOR_H
