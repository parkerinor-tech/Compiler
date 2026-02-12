// cFuncDeclNode.h
// Defines an AST node representing a function declaration or definition.

#pragma once
#include "cDeclNode.h"
#include "cFuncHeaderNode.h"
#include "cStmtsNode.h"
#include "cDeclsNode.h"

// Represents a function prototype or full function definition
class cFuncDeclNode : public cDeclNode
{
public:
    // Constructor for function prototype (header only)
    cFuncDeclNode(cFuncHeaderNode* header)
    {
        if (header->GetType()) AddChild(header->GetType());
        if (header->GetName()) AddChild(header->GetName());
        if (header->GetArgs()) AddChild(header->GetArgs());
    }

    // Constructor for full definition with local declarations and statements
    cFuncDeclNode(cFuncHeaderNode* header, cDeclsNode* decls, cStmtsNode* stmts)
    {
        if (header->GetType()) AddChild(header->GetType());
        if (header->GetName()) AddChild(header->GetName());
        if (header->GetArgs()) AddChild(header->GetArgs());
        if (decls) AddChild(decls);
        if (stmts) AddChild(stmts);
    }

    // Constructor for definition with statements only
    cFuncDeclNode(cFuncHeaderNode* header, cStmtsNode* stmts)
    {
        if (header->GetType()) AddChild(header->GetType());
        if (header->GetName()) AddChild(header->GetName());
        if (header->GetArgs()) AddChild(header->GetArgs());
        if (stmts) AddChild(stmts);
    }

    // Returns node type identifier
    virtual string NodeType() { return "func"; }

    // Visitor pattern hook
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};
