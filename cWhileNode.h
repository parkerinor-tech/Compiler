// cWhileNode.h
//********************************************************
// Defines an AST node representing a while loop statement.
//
// Example: while (condition) { body }
//

#pragma once
#include "cStmtNode.h"
#include "cExprNode.h"

// Represents a while loop with a condition and body
class cWhileNode : public cStmtNode
{
public:
    // cond = loop condition
    // body = statement(s) executed while condition is true
    cWhileNode(cExprNode* cond, cStmtNode* body)
    {
        AddChild(cond);
        AddChild(body);
    }

    // Returns node type identifier
    virtual string NodeType() { return "while"; }

    // Visitor pattern hook
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};
