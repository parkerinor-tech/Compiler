#pragma once
//**************************************
// cOpNode.h
//
// Class for an operator (e.g. + - etc.)
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Jan. 18, 2016
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
        string result("");

        switch (m_op)
        {
            case NOT_EQUALS:
                result += "!=";
                break;
            case EQUALS:
                result += "==";
                break;
            case OR:
                result += "||";
                break;
            case AND:
                result += "&&";  // Fixed: should be "&&" not "AND"
                break;
            case GE:
                result += ">=";
                break;
            case '>':
                result += ">";
                break;
            case LE:
                result += "<=";
                break;
            case '<':
                result += "<";
                break;
            case '+':
                result += "+";
                break;
            case '-':
                result += "-";
                break;
            case '*':
                result += "*";
                break;
            case '/':
                result += "/";
                break;
            case '%':
                result += "%";
                break;
            default:
                result += (char)m_op;
                break;
        }
        return result;
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

