#include "CodeGenerator.h"
#include <stdexcept>

std::string CodeGenerator::generate(const Program& program) {
    for (const auto& stmt : program.statements) {
        visit(stmt.get());
    }
    assembly_code << "HALT\n"; // End of program
    return assembly_code.str();
}

// --- Statement Visitors ---

void CodeGenerator::visit(const Statement* stmt) {
    if (auto s = dynamic_cast<const VarDecl*>(stmt)) return visit(s);
    if (auto s = dynamic_cast<const Assignment*>(stmt)) return visit(s);
    if (auto s = dynamic_cast<const IfStatement*>(stmt)) return visit(s);
    if (auto s = dynamic_cast<const BlockStatement*>(stmt)) return visit(s);
    throw std::runtime_error("CodeGenerator Error: Unknown statement type");
}

void CodeGenerator::visit(const VarDecl* stmt) {
    // Allocate a memory address for the new variable
    variable_addresses[stmt->varName] = next_address++;
    assembly_code << "; Variable '" << stmt->varName << "' allocated at address " << variable_addresses[stmt->varName] << "\n";
}

void CodeGenerator::visit(const Assignment* stmt) {
    // First, evaluate the expression on the right-hand side.
    // The result of the expression will be in register A.
    visit(stmt->value.get());
    
    // Now, store the result from register A into the variable's memory location.
    int address = variable_addresses.at(stmt->varName);
    assembly_code << "MOV [" << address << "], A ; " << stmt->varName << " = A\n";
}

void CodeGenerator::visit(const IfStatement* stmt) {
    std::string end_if_label = newLabel();

    // The condition is a binary operation (e.g., c == 30)
    auto condition = dynamic_cast<const BinaryOp*>(stmt->condition.get());
    if (!condition) {
        throw std::runtime_error("CodeGenerator Error: If condition must be a binary operation");
    }

    // Evaluate the left and right sides of the comparison
    visit(condition->left.get());   // Result in A
    assembly_code << "MOV B, A\n";        // Move result to B to save it
    visit(condition->right.get());  // New result in A
    
    // Compare A and B
    assembly_code << "CMP A, B\n";

    // If the condition is false (A != B), jump past the 'if' body
    assembly_code << "JNZ " << end_if_label << " ; Jump if not equal\n";
    
    // If the condition was true, execute the body
    visit(stmt->body.get());

    // Label to mark the end of the if statement
    assembly_code << end_if_label << ":\n";
}

void CodeGenerator::visit(const BlockStatement* stmt) {
    for (const auto& statement : stmt->statements) {
        visit(statement.get());
    }
}


// --- Expression Visitors ---

void CodeGenerator::visit(const Expression* expr) {
    if (auto e = dynamic_cast<const NumberLiteral*>(expr)) return visit(e);
    if (auto e = dynamic_cast<const Identifier*>(expr)) return visit(e);
    if (auto e = dynamic_cast<const BinaryOp*>(expr)) return visit(e);
    throw std::runtime_error("CodeGenerator Error: Unknown expression type");
}

void CodeGenerator::visit(const NumberLiteral* expr) {
    // Load the literal value into register A
    assembly_code << "MOV A, " << expr->value << "\n";
}

void CodeGenerator::visit(const Identifier* expr) {
    // Load the value from the variable's memory address into register A
    int address = variable_addresses.at(expr->name);
    assembly_code << "MOV A, [" << address << "]\n";
}

void CodeGenerator::visit(const BinaryOp* expr) {
    // Evaluate the right-hand side first and push its result onto a conceptual stack (here, register B)
    visit(expr->right.get());
    assembly_code << "MOV B, A\n"; // Store right side result in B

    // Evaluate the left-hand side, result is in A
    visit(expr->left.get());
    
    // Perform the operation
    if (expr->op == "+") {
        assembly_code << "ADD A, B\n";
    } else if (expr->op == "-") {
        assembly_code << "SUB A, B\n";
    } else if (expr->op != "==") { // '==' is handled inside the IfStatement visitor
        throw std::runtime_error("CodeGenerator Error: Unsupported binary operator '" + expr->op + "'");
    }
}

// --- Utility ---
std::string CodeGenerator::newLabel() {
    return "L" + std::to_string(label_counter++);
}
