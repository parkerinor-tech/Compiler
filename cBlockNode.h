#pragma once
//**************************************
// cBlockNode.h
//
// Defines AST node for a block of code (stuff inside {})
//
// Author: Phil Howard 
//

#include "cAstNode.h"
#include "cDeclsNode.h"
#include "cStmtNode.h"
#include "cStmtsNode.h"

class cBlockNode : public cStmtNode
{
    public:
        cBlockNode(cDeclsNode *decls, cStmtsNode *statements)
            : cStmtNode(), m_size(0)
        {
            AddChild(decls);
            AddChild(statements);
        }

        void SetSize(int size) { m_size = size; }
        int  GetSize()         { return m_size; }

        virtual string AttributesToString() override
        {
            if (m_size == 0) return "";
            return " size=\"" + std::to_string(m_size) + "\"";
        }

        virtual string NodeType() { return string("block"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        cDeclsNode *GetDecls()
        {
            return static_cast<cDeclsNode*>(GetChild(0));
        }
        cStmtsNode *GetStmts()
        {
            return static_cast<cStmtsNode*>(GetChild(1));
        }

    protected:
        int m_size;
};