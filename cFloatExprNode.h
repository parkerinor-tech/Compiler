#pragma once
#include "cExprNode.h"

class cFloatExprNode : public cExprNode
{
public:
    cFloatExprNode(float value) : m_value(value) {}

    virtual string NodeType() { return "float"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }

    virtual string AttributesToString()
    {
        return " value=\"" + std::to_string(m_value) + "\"";
    }

private:
    float m_value;
};

