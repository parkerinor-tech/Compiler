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

    cExprNode* GetLhs() { return dynamic_cast<cExprNode*>(GetChild(0)); }
    cOpNode*   GetOp()  { return dynamic_cast<cOpNode*>  (GetChild(1)); }
    cExprNode* GetRhs() { return dynamic_cast<cExprNode*>(GetChild(2)); }

    virtual cDeclNode* GetType() override
    {
        cExprNode* lhs = dynamic_cast<cExprNode*>(GetChild(0));
        if (lhs != nullptr) return lhs->GetType();
        return nullptr;
    }

    virtual string NodeType() { return "expr"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};