#pragma once
//**************************************
// cDeclNode.h
//
// Defines base class for all declarations.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"

class cDeclNode : public cAstNode
{
    public:
        cDeclNode() : cAstNode() {}

        // Virtual getters - subclasses override only the ones that apply
        virtual bool IsArray()  { return false; }
        virtual bool IsStruct() { return false; }
        virtual bool IsType()   { return false; }
        virtual bool IsFunc()   { return false; }
        virtual bool IsVar()    { return false; }
        virtual bool IsFloat()  { return false; }
        virtual bool IsInt()    { return false; }
        virtual bool IsChar()   { return false; }
        virtual int  GetSize()  { return 0; }

        // Every decl subclass must implement GetType()
        virtual cDeclNode* GetType() = 0;
};