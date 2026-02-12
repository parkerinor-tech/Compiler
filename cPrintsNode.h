// cPrintsNode.h
#pragma once
#include "cStmtNode.h"

class cPrintsNode : public cStmtNode
{
public:
    cPrintsNode(const std::string& str) : m_str(str) {}

    virtual string NodeType() { return "prints"; }

    virtual string AttributesToString() { return string(" value=\"") + m_str + "\""; }

    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }

protected:
    std::string m_str;
};

