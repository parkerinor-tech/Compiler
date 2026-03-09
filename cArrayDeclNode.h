// cArrayDeclNode.h
#pragma once
#include "cDeclNode.h"
#include "cSymbol.h"

class cArrayDeclNode : public cDeclNode
{
public:
    cArrayDeclNode(cSymbol* type, cSymbol* name, int size)
        : m_type(type), m_count(size)
    {
        AddChild(type);
        AddChild(name);
    }

    virtual bool IsArray() override { return true; }
    virtual bool IsType()  override { return true; }

    int GetCount() { return m_count; }
    cSymbol* GetElemTypeSym() { return dynamic_cast<cSymbol*>(GetChild(0)); }

    virtual string GetName() override
    {
        // child[1] is the name symbol
        cSymbol* sym = dynamic_cast<cSymbol*>(GetChild(1));
        if (sym != nullptr) return sym->GetName();
        return "";
    }

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
        return string(" count=\"") + std::to_string(m_count) + "\"";
    }

protected:
    cSymbol* m_type;
    int m_count;
};