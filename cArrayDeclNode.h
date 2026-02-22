// cArrayDeclNode.h
#pragma once
#include "cDeclNode.h"
#include "cSymbol.h"

class cArrayDeclNode : public cDeclNode
{
public:
    cArrayDeclNode(cSymbol* type, cSymbol* name, int size)
        : m_type(type), m_size(size)
    {
        AddChild(type);
        AddChild(name);
    }

    virtual bool IsArray() override { return true; }

    // The type of an array is the type of its element type symbol
    virtual cDeclNode* GetType() override
    {
        if (m_type != nullptr && m_type->GetDecl() != nullptr)
            return m_type->GetDecl()->GetType();
        return nullptr;
    }

    virtual string NodeType() { return "array_decl"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }

    virtual string AttributesToString()
    {
        return string(" count=\"") + std::to_string(m_size) + "\"";
    }

protected:
    cSymbol* m_type;
    int m_size;
};