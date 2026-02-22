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
    cVarExprNode(cSymbol* sym)
    {
        if (sym != nullptr)
        {
            // Check that the symbol has been declared (has a decl)
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

    virtual cDeclNode* GetType() override
    {
        cSymbol* sym = GetName();
        if (sym != nullptr && sym->GetDecl() != nullptr)
            return sym->GetDecl()->GetType();
        return nullptr;
    }

    virtual string NodeType() { return "varref"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }

protected:
    virtual string AttributesToString() { return ""; }
};