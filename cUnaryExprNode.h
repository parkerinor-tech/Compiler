// cUnaryExprNode.h
// Defines an AST node representing a unary expression (e.g., -x, !flag).

#pragma once
#include "cExprNode.h"
#include "cOpNode.h"

// Represents a unary expression with an operator and a single operand
class cUnaryExprNode : public cExprNode
{
public:
    // op   = unary operator (e.g., -, !)
    // expr = operand expression
    cUnaryExprNode(cOpNode* op, cExprNode* expr)
    {
        AddChild(op);
        AddChild(expr);
    }

    // Returns node type identifier
    virtual string NodeType() { return "unary"; }

    // Visitor pattern hook
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};
