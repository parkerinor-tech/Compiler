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

    virtual cDeclNode* GetType() override
    {
        cExprNode* expr = dynamic_cast<cExprNode*>(GetChild(1));
        if (expr != nullptr) return expr->GetType();
        return nullptr;
    }

    virtual string NodeType() { return "unary"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};