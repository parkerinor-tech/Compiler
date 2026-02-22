#pragma once
//********************************************************
// cSymbol.h - Define a class for symbols
//
// Author: Philip Howard
//
#include <string>
using std::string;

#include "cAstNode.h"

// Forward declaration to avoid circular includes
class cDeclNode;

class cSymbol : public cAstNode
{
    public:
        // Construct a symbol given its name
        cSymbol(string name) : cAstNode()
        {
            m_id = ++nextId;
            m_name = name;
            m_decl = nullptr;
        }

        virtual string AttributesToString() override
        {
            string result(" id=\"");
            result += std::to_string(m_id);
            result += "\" name=\"" + m_name + "\"";
            return result;
        }

        // Return name and ID of symbol
        string    GetName() { return m_name; }
        long long GetId()   { return m_id; }

        virtual string NodeType() override { return string("sym"); }
        virtual void Visit(cVisitor *visitor) override { visitor->Visit(this); }

        // Set/get the declaration node associated with this symbol
        void       SetDecl(cDeclNode* decl) { m_decl = decl; }
        cDeclNode* GetDecl() const          { return m_decl; }

    protected:
        static long long nextId;    // keeps track of unique symbol IDs
        long long m_id;             // Unique ID for this symbol
        string    m_name;           // Symbol name
        cDeclNode* m_decl;          // Declaration node for this symbol
};