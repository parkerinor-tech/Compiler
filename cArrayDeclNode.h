// cArrayDeclNode.h
#pragma once
#include "cDeclNode.h"
#include "cSymbol.h"

class cArrayDeclNode : public cDeclNode
{
public:
    cArrayDeclNode(cSymbol* type, cSymbol* name, int size)
        : m_size(size)
    {
        AddChild(type);
        AddChild(name);
    }

    virtual string NodeType() { return "array_decl"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }

    virtual string AttributesToString() { return string(" count=\"") + std::to_string(m_size) + "\""; }

protected:
    int m_size;
};

