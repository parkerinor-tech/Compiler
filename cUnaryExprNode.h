// cUnaryExprNode.h
#pragma once
#include "cExprNode.h"
#include "cOpNode.h"

class cUnaryExprNode : public cExprNode
{
public:
    cUnaryExprNode(cOpNode* op, cExprNode* expr)
    {
        AddChild(op);
        AddChild(expr);
    }

    virtual string NodeType() { return "unary"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};

