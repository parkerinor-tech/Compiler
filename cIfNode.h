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

    cExprNode*  GetCond()   { return dynamic_cast<cExprNode*> (GetChild(0)); }
    cStmtsNode* GetIfPart() { return dynamic_cast<cStmtsNode*>(GetChild(1)); }
    cStmtsNode* GetElse()   { return (GetNumChildren() > 2)
                                     ? dynamic_cast<cStmtsNode*>(GetChild(2))
                                     : nullptr; }

    virtual string NodeType() { return "if"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};