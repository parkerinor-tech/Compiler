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

    // Get the function name symbol (child 0)
    cSymbol* GetName()
    {
        return dynamic_cast<cSymbol*>(GetChild(0));
    }

    // Get the params node (child 1)
    cParamsNode* GetParams()
    {
        return dynamic_cast<cParamsNode*>(GetChild(1));
    }

    // Return type resolved by visiting the function's return type
    virtual cDeclNode* GetType() override
    {
        cSymbol* sym = GetName();
        if (sym == nullptr || sym->GetDecl() == nullptr) return nullptr;
        return sym->GetDecl()->GetType();
    }

    virtual string NodeType() { return "funcCall"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};