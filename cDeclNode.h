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

        virtual bool IsArray()  { return false; }
        virtual bool IsStruct() { return false; }
        virtual bool IsType()   { return false; }
        virtual bool IsFunc()   { return false; }
        virtual bool IsVar()    { return false; }
        virtual bool IsFloat()  { return false; }
        virtual bool IsInt()    { return false; }
        virtual bool IsChar()   { return false; }
        virtual int  GetSize()  { return 0; }

        virtual cDeclNode* GetType() = 0;
        virtual string GetName() = 0;

        // Returns true if 'other' can be assigned to 'this'.
        // Rules:
        //   - Same type: always compatible
        //   - Int (any size) can promote to float/double
        //   - Float cannot assign to int
        //   - Within same category: source size must be <= destination size
        bool IsCompatibleWith(cDeclNode* other)
        {
            if (other == nullptr) return false;
            cDeclNode* thisType  = this->GetType();
            cDeclNode* otherType = other->GetType();
            if (thisType == nullptr || otherType == nullptr) return false;
            if (thisType == otherType) return true;

            int thisSize  = thisType->GetSize();
            int otherSize = otherType->GetSize();

            // Float cannot be assigned to int category
            if (thisType->IsInt() && otherType->IsFloat()) return false;

            // Int can be assigned to float (promotion)
            if (thisType->IsFloat() && otherType->IsInt()) return true;

            // Within same category: source must fit in destination
            if (thisType->IsFloat() && otherType->IsFloat())
                return otherSize <= thisSize;

            if (thisType->IsInt() && otherType->IsInt())
                return otherSize <= thisSize;

            return false;
        }
};