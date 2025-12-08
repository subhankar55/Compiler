#ifndef AST_H
#define AST_H


#include <string>
#include <vector>
#include <memory>


struct Statement;
struct Expression;


struct Node {
    virtual ~Node() = default;
};


struct Expression : public Node {};


struct NumberLiteral : public Expression {
    int value;
    explicit NumberLiteral(int val) : value(val) {}
};


struct Identifier : public Expression {
    std::string name;
    explicit Identifier(std::string n) : name(std::move(n)) {}
};


struct BinaryOp : public Expression {
    std::string op;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    BinaryOp(std::string o, std::unique_ptr<Expression> l, std::unique_ptr<Expression> r)
        : op(std::move(o)), left(std::move(l)), right(std::move(r)) {}
};


struct Statement : public Node {};


struct VarDecl : public Statement {
    std::string varName;
    explicit VarDecl(std::string name) : varName(std::move(name)) {}
};


struct Assignment : public Statement {
    std::string varName;
    std::unique_ptr<Expression> value;
    Assignment(std::string name, std::unique_ptr<Expression> val)
        : varName(std::move(name)), value(std::move(val)) {}
};


struct BlockStatement : public Statement {
    std::vector<std::unique_ptr<Statement>> statements;
};


struct IfStatement : public Statement {
    std::unique_ptr<Expression> condition;
    std::unique_ptr<BlockStatement> body;
    IfStatement(std::unique_ptr<Expression> cond, std::unique_ptr<BlockStatement> b)
        : condition(std::move(cond)), body(std::move(b)) {}
};


struct Program : public Node {
    std::vector<std::unique_ptr<Statement>> statements;
};


#endif
