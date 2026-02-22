#pragma once
#include "cExprNode.h"
#include "cSymbolTable.h"

class cFloatExprNode : public cExprNode
{
public:
    cFloatExprNode(float value) : m_value(value) {}

    virtual cDeclNode* GetType() override
    {
        cSymbol* sym = g_symbolTable.Find("float");
        if (sym != nullptr) return sym->GetDecl();
        return nullptr;
    }

    virtual string NodeType() { return "float"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }

    virtual string AttributesToString()
    {
        return " value=\"" + std::to_string(m_value) + "\"";
    }

private:
    float m_value;
};