#pragma once
#include "cAstNode.h"
#include "cExprNode.h"

class cParamsNode : public cAstNode
{
public:
    cParamsNode(cAstNode* param) : m_size(0)
    {
        if (param != nullptr) AddChild(param);
    }

    void Add(cAstNode* param)       { if (param) AddChild(param); }
    cAstNode*  GetParam(int i)      { return GetChild(i); }
    cAstNode*  GetChildNode(int i)  { return GetChild(i); }
    void       SetSize(int size)    { m_size = size; }
    int        GetSize()            { return m_size; }

    virtual string NodeType() { return "params"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }

protected:
    virtual string AttributesToString() override
    {
        if (m_size == 0) return "";
        return " size=\"" + std::to_string(m_size) + "\"";
    }

private:
    int m_size;
};