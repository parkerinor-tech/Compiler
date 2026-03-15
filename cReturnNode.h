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

    cExprNode* GetExpr() { return dynamic_cast<cExprNode*>(GetChild(0)); }

    virtual string NodeType() { return "return"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};