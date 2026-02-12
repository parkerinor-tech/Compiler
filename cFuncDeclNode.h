#pragma once
#include "cDeclNode.h"
#include "cFuncHeaderNode.h"
#include "cStmtsNode.h"
#include "cDeclsNode.h"

class cFuncDeclNode : public cDeclNode
{
public:
    // Prototype (header only)
    cFuncDeclNode(cFuncHeaderNode* header)
    {
        if (header->GetType()) AddChild(header->GetType());
        if (header->GetName()) AddChild(header->GetName());
        if (header->GetArgs()) AddChild(header->GetArgs());
    }

    // Definition with decls and stmts
    cFuncDeclNode(cFuncHeaderNode* header, cDeclsNode* decls, cStmtsNode* stmts)
    {
        if (header->GetType()) AddChild(header->GetType());
        if (header->GetName()) AddChild(header->GetName());
        if (header->GetArgs()) AddChild(header->GetArgs());
        if (decls) AddChild(decls);
        if (stmts) AddChild(stmts);
    }

    // Definition with stmts only
    cFuncDeclNode(cFuncHeaderNode* header, cStmtsNode* stmts)
    {
        if (header->GetType()) AddChild(header->GetType());
        if (header->GetName()) AddChild(header->GetName());
        if (header->GetArgs()) AddChild(header->GetArgs());
        if (stmts) AddChild(stmts);
    }

    virtual string NodeType() { return "func"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};
