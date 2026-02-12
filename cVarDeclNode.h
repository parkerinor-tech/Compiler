// cVarDeclNode.h
//********************************************************
// Defines an AST node representing a variable declaration.
//
// Example: int x;
//

#ifndef CVARDECLNODE_H
#define CVARDECLNODE_H

#include "cDeclNode.h"
#include "cSymbol.h"

// Represents a variable declaration with a type and name
class cVarDeclNode : public cDeclNode
{
public:
    // type = variable type symbol
    // name = variable name symbol
    cVarDeclNode(cSymbol* type, cSymbol* name)
        : cDeclNode(), m_type(type), m_name(name)
    {
        AddChild(m_type);
        AddChild(m_name);
    }

    // Returns node type identifier
    virtual string NodeType() { return "var_decl"; }

    // Visitor pattern hook
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }

protected:
    cSymbol* m_type;  // Type of the variable
    cSymbol* m_name;  // Name of the variable
};

#endif
