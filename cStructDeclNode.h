// cStructDeclNode.h
#pragma once
#include "cDeclNode.h"
#include "cDeclsNode.h"
#include "cSymbol.h"

class cStructDeclNode : public cDeclNode
{
public:
    cStructDeclNode(cDeclsNode* decls, cSymbol* name)
    {
        if (decls != nullptr) AddChild(decls);
        if (name != nullptr) AddChild(name);
    }

    virtual bool IsStruct() override { return true; }
    virtual bool IsType()   override { return true; }

    virtual string GetName() override
    {
        // child[1] is the name symbol
        cSymbol* sym = dynamic_cast<cSymbol*>(GetChild(1));
        if (sym != nullptr) return sym->GetName();
        return "";
    }

    virtual cDeclNode* GetType() override { return this; }

    virtual string NodeType() { return "struct_decl"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};