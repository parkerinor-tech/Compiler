#pragma once
//**************************************
// cFuncExprNode.h
//
// AST node for a function call used as an expression.
//
// Author: Phil Howard 
//

#include "cAstNode.h"
#include "cExprNode.h"
#include "cAstNode.h"

class cFuncExprNode : public cExprNode
{
public:
    // param is the function call node
    cFuncExprNode(cAstNode* funcCall) : cExprNode()
    {
        m_funcCall = funcCall;
        AddChild(funcCall);
    }

    virtual string AttributesToString() 
    {
        return "";  // function calls themselves usually don't need extra attributes
    }

    virtual string NodeType() { return "func_expr"; }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

protected:
    cAstNode* m_funcCall;  // points to the function call node
};

