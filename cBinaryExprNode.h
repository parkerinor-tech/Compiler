// cBinaryExprNode.h
// Defines an AST node representing a binary expression (e.g., a + b).

#pragma once
#include "cExprNode.h"
#include "cOpNode.h"

// Represents a binary expression with a left operand, operator, and right operand
class cBinaryExprNode : public cExprNode
{
public:
    // lhs = left expression
    // op  = operator node
    // rhs = right expression
    cBinaryExprNode(cExprNode* lhs, cOpNode* op, cExprNode* rhs)
    {
        AddChild(lhs);
        AddChild(op);
        AddChild(rhs);
    }

    // Returns node type identifier
    virtual string NodeType() { return "expr"; }

    // Visitor pattern hook
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};
