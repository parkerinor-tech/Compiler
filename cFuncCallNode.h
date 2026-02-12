// cFuncCallNode.h
// Defines an AST node representing a function call expression.

#pragma once
#include "cExprNode.h"
#include "cSymbol.h"
#include "cParamsNode.h"

// Represents a function call (e.g., foo(a, b))
class cFuncCallNode : public cExprNode
{
public:
    // name   = function identifier
    // params = argument list
    cFuncCallNode(cSymbol* name, cParamsNode* params)
    {
        if (name) AddChild(name);
        if (params) AddChild(params);
    }

    // Returns node type identifier
    virtual string NodeType() { return "funcCall"; }

    // Visitor pattern hook
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};
