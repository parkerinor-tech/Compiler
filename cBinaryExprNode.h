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

    // Return the type of the left-hand side expression
    virtual cDeclNode* GetType() override
    {
        cExprNode* lhs = dynamic_cast<cExprNode*>(GetChild(0));
        if (lhs != nullptr) return lhs->GetType();
        return nullptr;
    }

    virtual string NodeType() { return "expr"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};