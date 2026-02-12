#ifndef CVARDECLNODE_H
#define CVARDECLNODE_H
// cVarDeclNode.h
#include "cDeclNode.h"
#include "cSymbol.h"

class cVarDeclNode : public cDeclNode
{
public:
    cVarDeclNode(cSymbol* type, cSymbol* name) : cDeclNode(), m_type(type), m_name(name) {
        AddChild(m_type);
        AddChild(m_name);
    }

    virtual string NodeType() { return "var_decl"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }

protected:
    cSymbol* m_type;
    cSymbol* m_name;
};
#endif
