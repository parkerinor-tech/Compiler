#pragma once
#include "cAstNode.h"

class cArgsNode : public cAstNode
{
public:
    cArgsNode(cAstNode* arg)
    {
        if (arg != nullptr) AddChild(arg);
    }

    // Public getter for children (used by cSemantics visitor)
    cAstNode* GetChildNode(int index) { return cAstNode::GetChild(index); }

    virtual string NodeType() { return "args"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};