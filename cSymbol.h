// cSymbol.h
//********************************************************
// Defines a class representing symbols in the AST (identifiers, types, etc.)
//
// Author: Philip Howard
//

#pragma once
#include <string>
#include "cAstNode.h"

using std::string;

// Represents a symbol with a name, unique ID, and optional type flag
class cSymbol : public cAstNode
{
public:
    // Construct a symbol given its name
    cSymbol(string name) : cAstNode()
    {
        m_id = ++nextId;    // Assign a unique ID
        m_name = name;      // Store symbol name
        m_isType = false;   // Default: not a type
    }

    // Return string representation of attributes for debugging/output
    virtual string AttributesToString() override
    {
        string result(" id=\"");
        result += std::to_string(m_id);
        result += "\" name=\"" + m_name + "\"";
        return result;
    }

    // Accessors
    string GetName() { return m_name; }
    long long GetId() { return m_id; }
    bool GetIsType() const { return m_isType; }
    void SetIsType(bool isType) { m_isType = isType; }

    // AST node type identifier
    virtual string NodeType() override { return string("sym"); }

    // Visitor pattern hook
    virtual void Visit(cVisitor* visitor) override { visitor->Visit(this); }

protected:
    static long long nextId;  // Tracks unique symbol IDs across all symbols
    long long m_id;            // Unique ID for this symbol
    string m_name;             // Symbol name
    bool m_isType;             // Flag indicating if this symbol represents a type
};
