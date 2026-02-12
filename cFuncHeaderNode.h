#pragma once
#include "cAstNode.h"
#include "cSymbol.h"
#include "cArgsNode.h"

class cFuncHeaderNode : public cAstNode
{
public:
    cFuncHeaderNode(cSymbol* type, cSymbol* name)
    {
        m_type = type;
        m_name = name;
        if (m_type) AddChild(m_type);
        if (m_name) AddChild(m_name);
    }

    void SetArgs(cArgsNode* args)
    {
        if (args) AddChild(args);
    }

    cSymbol* GetType() { if (GetNumChildren() > 0) return dynamic_cast<cSymbol*>(GetChildProtected(0)); return nullptr; }
    cSymbol* GetName() { if (GetNumChildren() > 1) return dynamic_cast<cSymbol*>(GetChildProtected(1)); return nullptr; }
    cArgsNode* GetArgs() { if (GetNumChildren() > 2) return dynamic_cast<cArgsNode*>(GetChildProtected(2)); return nullptr; }

    virtual string NodeType() { return "func_header"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }

protected:
    cSymbol* m_type;
    cSymbol* m_name;

    // helper to access protected child getter from here
    cAstNode* GetChildProtected(int index) { return cAstNode::GetChild(index); }
};
