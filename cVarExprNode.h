// cVarExprNode.h
#pragma once
#include "cExprNode.h"
#include "cSymbol.h"

class cVarExprNode : public cExprNode
{
public:

    // Add this constructor
    cVarExprNode(cSymbol* sym) 
    {
        if(sym !=nullptr)
            AddChild(sym);   // automatically add as child for XML
    }


    cSymbol* GetName()
    {
        if(NumChildren() >0)
            return dynamic_cast<cSymbol*>(GetChild(0));
        return nullptr;
    }

    virtual string NodeType() { return "varref"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }

protected:

    virtual string AttributesToString()
    {
        return "";
    }
};

