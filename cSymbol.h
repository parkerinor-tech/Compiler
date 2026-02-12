#pragma once
//********************************************************
// cSymbol.h - Define a class for symbols
//
// Author: Philip Howard
//
#include <string>

using std::string;

#include "cAstNode.h"

class cSymbol : public cAstNode
{
    public:
        // Construct a symbol given its name
        cSymbol(string name) : cAstNode()
        {
            m_id = ++nextId;
            m_name = name;
            m_isType=false;
        }


         virtual string AttributesToString() override
        {
            string result(" id=\"");
            result += std::to_string(m_id);
            result += "\" name=\"" + m_name + "\"";
            return result;
        }


        // Return name of symbol
        string GetName() { return m_name; }
        long long GetId() { return m_id; }
         virtual string NodeType() override { return string("sym"); }
        virtual void Visit(cVisitor *visitor)override { visitor->Visit(this); }
        void SetIsType(bool isType) { m_isType = isType; }
    bool GetIsType() const { return m_isType; }

    protected:
        static long long nextId;    // keeps track of unique symbol IDs
        long long m_id;             // Unique ID for this symbol
        string m_name;              // Symbol name
        bool m_isType;
};
