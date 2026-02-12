// cFuncHeaderNode.h
// Defines an AST node representing a function header (return type, name, and arguments).

#pragma once
#include "cAstNode.h"
#include "cSymbol.h"
#include "cArgsNode.h"

// Represents a function header (e.g., int foo(int a))
class cFuncHeaderNode : public cAstNode
{
public:
    // type = return type, name = function identifier
    cFuncHeaderNode(cSymbol* type, cSymbol* name)
    {
        m_type = type;
        m_name = name;
        if (m_type) AddChild(m_type);
        if (m_name) AddChild(m_name);
    }

    // Adds argument list to the header
    void SetArgs(cArgsNode* args)
    {
        if (args) AddChild(args);
    }

    // Accessors for return type, name, and arguments
    cSymbol* GetType()
    {
        if (GetNumChildren() > 0)
            return dynamic_cast<cSymbol*>(GetChildProtected(0));
        return nullptr;
    }

    cSymbol* GetName()
    {
        if (GetNumChildren() > 1)
            return dynamic_cast<cSymbol*>(GetChildProtected(1));
        return nullptr;
    }

    cArgsNode* GetArgs()
    {
        if (GetNumChildren() > 2)
            return dynamic_cast<cArgsNode*>(GetChildProtected(2));
        return nullptr;
    }

    // Returns node type identifier
    virtual string NodeType() { return "func_header"; }

    // Visitor pattern hook
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }

protected:
    cSymbol* m_type;  // Return type symbol
    cSymbol* m_name;  // Function name symbol

    // Helper to access protected child getter
    cAstNode* GetChildProtected(int index)
    {
        return cAstNode::GetChild(index);
    }
};
