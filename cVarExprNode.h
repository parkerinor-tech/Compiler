#pragma once
//**************************************
// cVarExprNode.h
//
// Defines an AST node for a variable reference.
// This is used when a variable is used in an expression.
//
// Inherits from cExprNode so that it can appear anywhere
// expressions are used.
//
// Author: Parker Fagen
//

#include "cAstNode.h"
#include "cExprNode.h"
//include "cVarRefNode.h"   

class cVarExprNode : public cExprNode
{
public:
    // Constructor: takes a varref AST node (the variable being referenced)
    cVarExprNode(cAstNode *varRef) : cExprNode()
    {
        m_varRef = varRef;
        AddChild(m_varRef);   // add the varref as a child node
    }

    virtual string NodeType() { return "var_expr"; }
    
    virtual string AttributesToString()
    {
        return "";  // usually variable references don't need extra attributes
    }

    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

protected:
    cAstNode *m_varRef;   // pointer to the variable reference AST node
};
