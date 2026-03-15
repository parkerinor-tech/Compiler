#pragma once
#include "cAstNode.h"
#include "cStmtNode.h"
#include "cExprNode.h"

class cPrintNode : public cStmtNode
{
public:
    cPrintNode(cExprNode *expr) : cStmtNode()
    {
        AddChild(expr);
    }

    cExprNode* GetExpr() { return dynamic_cast<cExprNode*>(GetChild(0)); }

    virtual string NodeType() { return string("print"); }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};