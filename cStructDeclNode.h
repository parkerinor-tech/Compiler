// cStructDeclNode.h
// Defines an AST node representing a struct declaration.

#pragma once
#include "cAstNode.h"
#include "cDeclsNode.h"
#include "cSymbol.h"

// Represents a struct declaration with fields and a name
class cStructDeclNode : public cAstNode
{
public:
    // decls = fields inside the struct
    // name  = struct identifier
    cStructDeclNode(cDeclsNode* decls, cSymbol* name)
    {
        if (decls != nullptr) AddChild(decls);
        if (name != nullptr) AddChild(name);
    }

    // Returns node type identifier
    virtual string NodeType() { return "struct_decl"; }

    // Visitor pattern hook
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};
