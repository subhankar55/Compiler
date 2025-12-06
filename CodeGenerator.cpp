#include "CodeGenerator.h"
#include <stdexcept>

std::string CodeGenerator::generate(const Program& program) {
    for (const auto& stmt : program.statements) {
        visit(stmt.get());
    }
    assembly_code << "hlt\n"; // End of program
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
    // Allocate a memory address for the new variable. No assembly is emitted yet.
    variable_addresses[stmt->varName] = next_address++;
    assembly_code << "; Variable '" << stmt->varName << "' allocated at address " << variable_addresses[stmt->varName] << "\n";
}

void CodeGenerator::visit(const Assignment* stmt) {
    // Evaluate the expression on the right-hand side.
    // The result of the expression will be in register A.
    visit(stmt->value.get());
    
    // Now, store the result from register A into the variable's memory location using 'sta'.
    int address = variable_addresses.at(stmt->varName);
    assembly_code << "sta " << address << " ; " << stmt->varName << " = A\n";
}

void CodeGenerator::visit(const IfStatement* stmt) {
    std::string end_if_label = newLabel();

    auto condition = dynamic_cast<const BinaryOp*>(stmt->condition.get());
    if (!condition || condition->op != "==") {
        throw std::runtime_error("CodeGenerator Error: If condition must be an equality '==' check");
    }

    // Evaluate the left and right sides of the comparison.
    visit(condition->left.get());   // Result in A
    assembly_code << "push A\n";          // Push the left-side result onto the stack
    visit(condition->right.get());  // Right-side result is now in A
    assembly_code << "pop B\n";           // Pop the left-side result into B
    
    // Compare A (right side) and B (left side)
    // NOTE: The 'cmp' instruction in your spec (A-B) means we compare A against B.
    // To match this, let's load the right side into B and left side into A.
    // Re-evaluating for clarity:
    assembly_code.str(""); // Clear previous assembly for this node
    visit(condition->left.get()); // Left side value in A
    assembly_code << "push A\n";
    visit(condition->right.get()); // Right side value in A
    assembly_code << "mov B A\n"; // Move right side to B
    assembly_code << "pop A\n"; // Pop left side into A
    
    assembly_code << "cmp\n"; // Compare A and B

    // If A != B (zero flag not set), jump past the 'if' body. 'jne' is an alias for 'jnz'.
    assembly_code << "jne " << end_if_label << " ; Jump if not equal\n";
    
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
    // Load the literal value into register A using 'ldi'
    assembly_code << "ldi A " << expr->value << "\n";
}

void CodeGenerator::visit(const Identifier* expr) {
    // Load the value from the variable's memory address into register A using 'lda'
    int address = variable_addresses.at(expr->name);
    assembly_code << "lda " << address << "\n";
}

void CodeGenerator::visit(const BinaryOp* expr) {
    // Evaluate the left-hand side and push its result onto the stack
    visit(expr->left.get());
    assembly_code << "push A\n"; 

    // Evaluate the right-hand side, result is in A
    visit(expr->right.get());
    assembly_code << "mov B A\n"; // Store right side result in B
    
    // Pop the left-hand side result from the stack into A
    assembly_code << "pop A\n";
    
    // Perform the operation
    if (expr->op == "+") {
        assembly_code << "add\n";
    } else if (expr->op == "-") {
        assembly_code << "sub\n";
    } else if (expr->op != "==") { // '==' is handled inside the IfStatement visitor
        throw std::runtime_error("CodeGenerator Error: Unsupported binary operator '" + expr->op + "'");
    }
}

// --- Utility ---
std::string CodeGenerator::newLabel() {
    return "L" + std::to_string(label_counter++);
}
