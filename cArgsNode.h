#pragma once
#include "cAstNode.h"

class cArgsNode : public cAstNode
{
public:
    cArgsNode(cAstNode* arg)
    {
        if (arg != nullptr) AddChild(arg);
    }

    virtual string NodeType() { return "args"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};
