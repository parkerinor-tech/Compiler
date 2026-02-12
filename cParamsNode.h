#pragma once
#include "cAstNode.h"

class cParamsNode : public cAstNode
{
public:
    cParamsNode(cAstNode* param)
    {
        if (param != nullptr) AddChild(param);
    }

    virtual string NodeType() { return "params"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};
