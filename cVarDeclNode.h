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
        cSymbol* existing = g_symbolTable.FindLocal(name->GetName());
        if (existing != nullptr && existing->GetDecl() != nullptr)
        {
            // Already properly declared in this scope - duplicate error
            SemanticParseError("Symbol " + name->GetName() +
                               " already defined in current scope");
            name->SetDecl(existing->GetDecl());
        }
        else if (existing != nullptr)
        {
            // Lexer inserted this symbol into current scope (no decl yet)
            // Set decl directly on this symbol - same object, same ID
            existing->SetDecl(this);
            name = existing;
        }
        else
        {
            // Symbol not in current scope at all - came from outer scope
            // Create a fresh symbol with a new ID for this inner scope
            cSymbol* fresh = new cSymbol(name->GetName());
            fresh->SetDecl(this);
            g_symbolTable.Insert(fresh);
            name = fresh;
        }

        m_type = type;
        m_name = name;
        AddChild(m_type);
        AddChild(m_name);
    }

    virtual bool IsVar() override { return true; }

    virtual bool IsArray() override 
    { 
        return m_type != nullptr && 
               m_type->GetDecl() != nullptr && 
               m_type->GetDecl()->IsArray(); 
    }

    virtual string GetName() override
    {
        if (m_name != nullptr) return m_name->GetName();
        return "";
    }

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