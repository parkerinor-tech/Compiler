// cReturnNode.h
// Defines an AST node representing a return statement with an optional expression.

#pragma once
#include "cStmtNode.h"
#include "cExprNode.h"

// Represents a return statement (e.g., return expr;)
class cReturnNode : public cStmtNode
{
public:
    // expr = expression being returned
    cReturnNode(cExprNode* expr)
    {
        AddChild(expr);
    }

    // Returns node type identifier
    virtual string NodeType() { return "return"; }

    // Visitor pattern hook
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};
