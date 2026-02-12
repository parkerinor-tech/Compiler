// cFloatExprNode.h
// Defines an AST node representing a floating-point literal.

#pragma once
#include "cExprNode.h"

// Represents a float constant in an expression (e.g., 3.14)
class cFloatExprNode : public cExprNode
{
public:
    // value = floating-point literal value
    cFloatExprNode(float value) : m_value(value) {}

    // Returns node type identifier
    virtual string NodeType() { return "float"; }

    // Visitor pattern hook
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }

    // Returns formatted attribute string for output/debugging
    virtual string AttributesToString()
    {
        return " value=\"" + std::to_string(m_value) + "\"";
    }

private:
    float m_value; // Stored float literal value
};
