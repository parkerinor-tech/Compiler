#pragma once
//**************************************
// cDeclsNode.h
//
// Defines a class to represent a list of declarations.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cDeclNode.h"

class cDeclsNode : public cAstNode
{
    public:
        cDeclsNode(cDeclNode *decl) : cAstNode(), m_size(0)
        {
            AddChild(decl);
        }

        void Insert(cDeclNode *decl) { AddChild(decl); }

        void SetSize(int size) { m_size = size; }
        int  GetSize()         { return m_size; }

        virtual string AttributesToString() override
        {
            if (m_size == 0) return string("");
            return string(" size=\"") + std::to_string(m_size) + string("\"");
        }

        virtual string NodeType() { return string("decls"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        int m_size;
};