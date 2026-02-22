#pragma once
//**************************************
// cIntExprNode.h
//
// Defines an AST node for an integer constant (literals).
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cExprNode.h"
#include "cSymbolTable.h"

class cIntExprNode : public cExprNode
{
    public:
        cIntExprNode(int value) : cExprNode()
        {
            m_value = value;
        }

        // If value fits in a char (0..127), return char type; otherwise int
        virtual cDeclNode* GetType() override
        {
            string typeName = (m_value >= 0 && m_value <= 127) ? "char" : "int";
            cSymbol* sym = g_symbolTable.Find(typeName);
            if (sym != nullptr) return sym->GetDecl();
            return nullptr;
        }

        virtual string AttributesToString() 
        {
            return " value=\"" + std::to_string(m_value) + "\"";
        }

        virtual string NodeType() { return string("int"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        int m_value;
};