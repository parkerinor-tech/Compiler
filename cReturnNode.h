// cReturnNode.h
#pragma once
#include "cStmtNode.h"
#include "cExprNode.h"

class cReturnNode : public cStmtNode
{
public:
    cReturnNode(cExprNode* expr)
    {
        AddChild(expr);
    }

    virtual string NodeType() { return "return"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};

