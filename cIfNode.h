// cIfNode.h
#pragma once
#include "cStmtNode.h"
#include "cExprNode.h"
#include "cStmtsNode.h"

class cIfNode : public cStmtNode
{
public:
    cIfNode(cExprNode* cond, cStmtsNode* ifPart, cStmtsNode* elsePart = nullptr)
    {
        AddChild(cond);
        AddChild(ifPart);
        if (elsePart) AddChild(elsePart);
    }

    virtual string NodeType() { return "if"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};

