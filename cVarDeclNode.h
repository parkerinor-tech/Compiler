#pragma once
//**************************************
// cVarDeclNode.h
//
// AST node for a variable declaration
//
// Inherits from cAstNode. Stores the type and symbol name.
//
// Author: Parker Fagen
//

#include "cAstNode.h"
#include "cDeclNode.h"
#include "cSymbol.h"

class cVarDeclNode : public cDeclNode
{
public:
    cVarDeclNode(cSymbol *type, cSymbol *name) : cDeclNode()
    {
        m_type = type;
        m_name = name;
    }

    virtual string NodeType() { return "var_decl"; }

    virtual string AttributesToString()
    {
        string result = " type=\"" + m_type->GetName() + "\"";
        result += " name=\"" + m_name->GetName() + "\"";
        return result;
    }

    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

protected:
    cSymbol *m_type;   // type of the variable (e.g. int, float)
    cSymbol *m_name;   // name of the variable
};

