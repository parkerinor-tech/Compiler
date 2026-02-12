// cStructDeclNode.h
#pragma once
#include "cAstNode.h"
#include "cDeclsNode.h"
#include "cSymbol.h"

class cStructDeclNode : public cAstNode
{
public:
    cStructDeclNode(cDeclsNode* decls, cSymbol* name)
    {
        if (decls != nullptr) AddChild(decls);
        if (name != nullptr) AddChild(name);
    }

    virtual string NodeType() { return "struct_decl"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};
