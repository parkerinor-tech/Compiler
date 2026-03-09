#pragma once
//**************************************
// cVarExprNode.h
//
// Checks that a referenced variable has been declared.
//
#include "cExprNode.h"
#include "cSymbol.h"
#include "cSymbolTable.h"

class cVarExprNode : public cExprNode
{
public:
    cVarExprNode(cSymbol* sym) : m_size(0), m_offset(0)
    {
        if (sym != nullptr)
        {
            cSymbol* found = g_symbolTable.Find(sym->GetName());
            if (found == nullptr || found->GetDecl() == nullptr)
            {
                SemanticParseError("Symbol " + sym->GetName() + " not defined");
            }
            AddChild(sym);
        }
    }

    cSymbol* GetName()
    {
        if (NumChildren() > 0)
            return dynamic_cast<cSymbol*>(GetChild(0));
        return nullptr;
    }

    // Public getter for children (used by cSemantics visitor)
    cAstNode* GetChildNode(int index) { return cExprNode::GetChild(index); }

    virtual cDeclNode* GetType() override
    {
        cSymbol* sym = GetName();
        if (sym != nullptr && sym->GetDecl() != nullptr)
            return sym->GetDecl()->GetType();
        return nullptr;
    }

    // Size and offset for varref (computed by cComputeSize)
    void SetSize(int size)       { m_size = size; }
    void SetOffset(int offset)   { m_offset = offset; }
    int  GetVarSize()            { return m_size; }
    int  GetOffset()             { return m_offset; }

    // Row sizes for array references (comma-separated string)
    void SetRowSizes(const string& rs) { m_rowSizes = rs; }
    string GetRowSizes()               { return m_rowSizes; }

    virtual string NodeType() { return "varref"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }

protected:
    virtual string AttributesToString() override
    {
        if (m_size == 0 && m_offset == 0) return "";
        string result = " size=\"" + std::to_string(m_size) + "\"";
        result += " offset=\"" + std::to_string(m_offset) + "\"";
        if (!m_rowSizes.empty())
            result += " rowsizes=\"" + m_rowSizes + "\"";
        return result;
    }

    int    m_size;
    int    m_offset;
    string m_rowSizes;
};