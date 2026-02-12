

// cAssignNode.h
// Defines an AST node representing an assignment statement.

#pragma once
#include "cStmtNode.h"
#include "cVarExprNode.h"
#include "cExprNode.h"

// Represents an assignment (e.g., x = expr)
class cAssignNode : public cStmtNode
{
public:
    // lhs = variable being assigned
    // rhs = expression assigned to the variable
    cAssignNode(cVarExprNode* lhs, cExprNode* rhs)
    {
        AddChild(lhs);
        AddChild(rhs);
    }

    // Returns node type identifier
    virtual string NodeType() { return "assign"; }

    // Visitor pattern hook
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};