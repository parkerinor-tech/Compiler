#pragma once
#include "cExprNode.h"
#include "cSymbol.h"
#include "cParamsNode.h"

class cFuncCallNode : public cExprNode
{
public:
    cFuncCallNode(cSymbol* name, cParamsNode* params)
    {
        if (name) AddChild(name);
        if (params) AddChild(params);
    }

    // Return type will be fully resolved in Lab 5B via visitor
    virtual cDeclNode* GetType() override { return nullptr; }

    virtual string NodeType() { return "funcCall"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};