#include "CodeGenerator.h"
#include <stdexcept>


std::string CodeGenerator::generate(const Program& program) {
    for (const auto& stmt : program.statements) {
        visit(stmt.get());
    }
    assembly_code << "hlt\n"; 
    return assembly_code.str();
}



void CodeGenerator::visit(const Statement* stmt) {
    if (auto s = dynamic_cast<const VarDecl*>(stmt)) return visit(s);
    if (auto s = dynamic_cast<const Assignment*>(stmt)) return visit(s);
    if (auto s = dynamic_cast<const IfStatement*>(stmt)) return visit(s);
    if (auto s = dynamic_cast<const BlockStatement*>(stmt)) return visit(s);
    throw std::runtime_error("CodeGenerator Error: Unknown statement type");
}


void CodeGenerator::visit(const VarDecl* stmt) {
    variable_addresses[stmt->varName] = next_address++;
    assembly_code << "; Variable '" << stmt->varName << "' allocated at address " << variable_addresses[stmt->varName] << "\n";
}


void CodeGenerator::visit(const Assignment* stmt) {
    visit(stmt->value.get());
   
    int address = variable_addresses.at(stmt->varName);
    assembly_code << "sta " << address << " ; " << stmt->varName << " = A\n";
}


void CodeGenerator::visit(const IfStatement* stmt) {
    std::string end_if_label = newLabel();


    auto condition = dynamic_cast<const BinaryOp*>(stmt->condition.get());
    if (!condition || condition->op != "==") {
        throw std::runtime_error("CodeGenerator Error: If condition must be an equality '==' check");
    }


    
    visit(condition->left.get());   
    assembly_code << "push A\n";          
    visit(condition->right.get());  
    assembly_code << "pop B\n";           
   
   
    assembly_code.str(""); 
    visit(condition->left.get()); 
    assembly_code << "push A\n";
    visit(condition->right.get()); 
    assembly_code << "mov B A\n"; 
    assembly_code << "pop A\n"; 
   
    assembly_code << "cmp\n"; 


    assembly_code << "jne " << end_if_label << " ; Jump if not equal\n";
   
    visit(stmt->body.get());


    assembly_code << end_if_label << ":\n";
}


void CodeGenerator::visit(const BlockStatement* stmt) {
    for (const auto& statement : stmt->statements) {
        visit(statement.get());
    }
}




void CodeGenerator::visit(const Expression* expr) {
    if (auto e = dynamic_cast<const NumberLiteral*>(expr)) return visit(e);
    if (auto e = dynamic_cast<const Identifier*>(expr)) return visit(e);
    if (auto e = dynamic_cast<const BinaryOp*>(expr)) return visit(e);
    throw std::runtime_error("CodeGenerator Error: Unknown expression type");
}


void CodeGenerator::visit(const NumberLiteral* expr) {
    assembly_code << "ldi A " << expr->value << "\n";
}


void CodeGenerator::visit(const Identifier* expr) {
    int address = variable_addresses.at(expr->name);
    assembly_code << "lda " << address << "\n";
}


void CodeGenerator::visit(const BinaryOp* expr) {
    visit(expr->left.get());
    assembly_code << "push A\n";

    visit(expr->right.get());
    assembly_code << "mov B A\n"; 
   
    
    assembly_code << "pop A\n";
   
    
    if (expr->op == "+") {
        assembly_code << "add\n";
    } else if (expr->op == "-") {
        assembly_code << "sub\n";
    } else if (expr->op != "==") { 
        throw std::runtime_error("CodeGenerator Error: Unsupported binary operator '" + expr->op + "'");
    }
}


std::string CodeGenerator::newLabel() {
    return "L" + std::to_string(label_counter++);
}
