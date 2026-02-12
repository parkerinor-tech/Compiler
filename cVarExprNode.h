// cVarExprNode.h
//********************************************************
// Defines an AST node representing a variable reference in an expression.
//
// Example: x in "x + 5"
//

#pragma once
#include "cExprNode.h"
#include "cSymbol.h"

// Represents a reference to a variable
class cVarExprNode : public cExprNode
{
public:
    // Constructor: sym = symbol for the variable
    // Automatically adds symbol as child for tree structure / XML output
    cVarExprNode(cSymbol* sym)
    {
        if (sym != nullptr)
            AddChild(sym);
    }

    // Returns the variable's symbol (name)
    cSymbol* GetName()
    {
        if (NumChildren() > 0)
            return dynamic_cast<cSymbol*>(GetChild(0));
        return nullptr;
    }

    // Returns node type identifier
    virtual string NodeType() { return "varref"; }

    // Visitor pattern hook
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }

protected:
    // No additional attributes for XML
    virtual string AttributesToString()
    {
        return "";
    }
};
