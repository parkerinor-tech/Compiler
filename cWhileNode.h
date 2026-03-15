#pragma once
#include "cStmtNode.h"
#include "cExprNode.h"

class cWhileNode : public cStmtNode
{
public:
    cWhileNode(cExprNode* cond, cStmtNode* body)
    {
        AddChild(cond);
        AddChild(body);
    }

    cExprNode* GetCond() { return dynamic_cast<cExprNode*>(GetChild(0)); }
    cStmtNode* GetBody() { return dynamic_cast<cStmtNode*>(GetChild(1)); }

    virtual string NodeType() { return "while"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};