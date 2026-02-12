#pragma once
#include "cExprNode.h"
#include "cOpNode.h"

class cBinaryExprNode : public cExprNode
{
public:
    cBinaryExprNode(cExprNode* lhs, cOpNode* op, cExprNode* rhs)
    {
        AddChild(lhs);
        AddChild(op);
        AddChild(rhs);
    }

    virtual string NodeType() { return "expr"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};

