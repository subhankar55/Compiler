#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>

// Forward declarations
struct Statement;
struct Expression;

// Base class for all AST nodes
struct Node {
    virtual ~Node() = default;
};

// Base class for all expression nodes
struct Expression : public Node {};

// Expression for an integer literal (e.g., 10)
struct NumberLiteral : public Expression {
    int value;
    explicit NumberLiteral(int val) : value(val) {}
};

// Expression for an identifier (e.g., a, b)
struct Identifier : public Expression {
    std::string name;
    explicit Identifier(std::string n) : name(std::move(n)) {}
};

// Expression for a binary operation (e.g., a + b)
struct BinaryOp : public Expression {
    std::string op;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    BinaryOp(std::string o, std::unique_ptr<Expression> l, std::unique_ptr<Expression> r)
        : op(std::move(o)), left(std::move(l)), right(std::move(r)) {}
};

// Base class for all statement nodes
struct Statement : public Node {};

// Statement for a variable declaration (e.g., int a;)
struct VarDecl : public Statement {
    std::string varName;
    explicit VarDecl(std::string name) : varName(std::move(name)) {}
};

// Statement for an assignment (e.g., a = 10;)
struct Assignment : public Statement {
    std::string varName;
    std::unique_ptr<Expression> value;
    Assignment(std::string name, std::unique_ptr<Expression> val)
        : varName(std::move(name)), value(std::move(val)) {}
};

// Statement for a block of code { ... }
struct BlockStatement : public Statement {
    std::vector<std::unique_ptr<Statement>> statements;
};

// Statement for a conditional (e.g., if (c == 30) { ... })
struct IfStatement : public Statement {
    std::unique_ptr<Expression> condition;
    std::unique_ptr<BlockStatement> body;
    IfStatement(std::unique_ptr<Expression> cond, std::unique_ptr<BlockStatement> b)
        : condition(std::move(cond)), body(std::move(b)) {}
};

// The root of the AST, representing the whole program
struct Program : public Node {
    std::vector<std::unique_ptr<Statement>> statements;
};

#endif // AST_H
