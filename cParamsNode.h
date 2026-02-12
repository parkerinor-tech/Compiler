// cParamsNode.h
// Defines an AST node representing a list of parameters in a function call.

#pragma once
#include "cAstNode.h"

// Represents parameters passed to a function (e.g., foo(a, b, c))
class cParamsNode : public cAstNode
{
public:
    // param = single parameter expression
    cParamsNode(cAstNode* param)
    {
        if (param != nullptr) AddChild(param);
    }

    // Returns node type identifier
    virtual string NodeType() { return "params"; }

    // Visitor pattern hook
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};
