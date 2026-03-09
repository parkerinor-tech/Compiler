#pragma once
#include "cAstNode.h"

class cParamsNode : public cAstNode
{
public:
    cParamsNode(cAstNode* param) : m_size(0)
    {
        if (param != nullptr) AddChild(param);
    }

    void SetSize(int size) { m_size = size; }
    int  GetSize()         { return m_size; }

    // Public getter for children (used by cSemantics visitor)
    cAstNode* GetChildNode(int index) { return cAstNode::GetChild(index); }

    virtual string AttributesToString() override
    {
        if (m_size == 0) return "";
        return " size=\"" + std::to_string(m_size) + "\"";
    }

    virtual string NodeType() { return "params"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }

protected:
    int m_size;
};