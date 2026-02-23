#pragma once
//**************************************
// cOpNode.h
//
// Class for an operator (e.g. + - etc.)
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "langparse.h"
#include <string>

using std::string;

class cOpNode : public cAstNode
{
public:
    cOpNode(int op) : m_op(op) {}
    
    string GetOpAsString() const
    {
        switch (m_op)
        {
            case NOT_EQUALS: return "!=";
            case EQUALS:     return "==";
            case OR:         return "||";
            case AND:        return "&amp;&amp;";
            case GE:         return ">=";
            case '>':        return ">";
            case LE:         return "&lt;=";
            case '<':        return "&lt;";
            case '+':        return "+";
            case '-':        return "-";
            case '*':        return "*";
            case '/':        return "/";
            case '%':        return "%";
            default:         return string(1, (char)m_op);
        }
    }

    int GetOp() const { return m_op; }

    virtual string NodeType() { return "op"; }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    
protected:
    virtual string AttributesToString()
    {
        return " value=\"" + GetOpAsString() + "\"";
    }

private:
    int m_op;
};