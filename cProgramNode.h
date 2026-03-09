#pragma once
//**************************************
// cProgramNode.h
//
// Defines AST node for a lang program
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cBlockNode.h"

class cProgramNode : public cAstNode
{
    public:
        cProgramNode(cBlockNode *block) : cAstNode(), m_size(0)
        {
            AddChild(block);
        }

        void SetSize(int size) { m_size = size; }
        int  GetSize()         { return m_size; }

        cBlockNode* GetBlock()
        {
            return dynamic_cast<cBlockNode*>(GetChild(0));
        }

        virtual string AttributesToString() override
        {
            if (m_size == 0) return string("");
            return string(" size=\"") + std::to_string(m_size) + string("\"");
        }

        virtual string NodeType() { return string("program"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        int m_size;
};