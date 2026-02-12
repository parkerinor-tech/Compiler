// cPrintsNode.h
// Defines an AST node representing a print statement with a string literal.

#pragma once
#include "cStmtNode.h"

// Represents a print statement (e.g., print "Hello, world")
class cPrintsNode : public cStmtNode
{
public:
    // str = string to print
    cPrintsNode(const std::string& str) : m_str(str) {}

    // Returns node type identifier
    virtual string NodeType() { return "prints"; }

    // Returns string representation of the node's value
    virtual string AttributesToString() { return string(" value=\"") + m_str + "\""; }

    // Visitor pattern hook
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }

protected:
    std::string m_str; // Stored string literal
};
