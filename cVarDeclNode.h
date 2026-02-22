#pragma once
//**************************************
// cVarDeclNode.h
//
// Checks for duplicate declarations in the current scope.
//
#include "cDeclNode.h"
#include "cSymbol.h"
#include "cSymbolTable.h"

class cVarDeclNode : public cDeclNode
{
public:
    cVarDeclNode(cSymbol* type, cSymbol* name) : cDeclNode()
    {
        // Check for duplicate definition in current scope
        if (g_symbolTable.FindLocal(name->GetName()) != nullptr)
        {
            SemanticParseError("Symbol " + name->GetName() +
                               " already defined in current scope");
        }
        else
        {
            // Associate this decl with the symbol and insert it
            name->SetDecl(this);
            g_symbolTable.Insert(name);
        }

        m_type = type;
        m_name = name;
        AddChild(m_type);
        AddChild(m_name);
    }

    virtual bool IsVar() override { return true; }

    // The type of a variable is the type of its type-symbol's decl
    virtual cDeclNode* GetType() override
    {
        if (m_type != nullptr && m_type->GetDecl() != nullptr)
            return m_type->GetDecl()->GetType();
        return nullptr;
    }

    virtual string NodeType() { return "var_decl"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }

protected:
    cSymbol* m_type;
    cSymbol* m_name;
};