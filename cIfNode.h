// cIfNode.h
// Defines an AST node representing an if statement, optionally with an else branch.

#pragma once
#include "cStmtNode.h"
#include "cExprNode.h"
#include "cStmtsNode.h"

// Represents an if statement: if (condition) { ifPart } else { elsePart }
class cIfNode : public cStmtNode
{
public:
    // cond     = condition expression
    // ifPart   = statements executed if condition is true
    // elsePart = optional statements executed if condition is false
    cIfNode(cExprNode* cond, cStmtsNode* ifPart, cStmtsNode* elsePart = nullptr)
    {
        AddChild(cond);
        AddChild(ifPart);
        if (elsePart) AddChild(elsePart);
    }

    // Returns node type identifier
    virtual string NodeType() { return "if"; }

    // Visitor pattern hook
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};
