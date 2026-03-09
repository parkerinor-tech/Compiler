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
        cDeclNode() : cAstNode(), m_size(0), m_offset(0) {}

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

        // Size and offset getters/setters (per lab rules: no computation here)
        void SetSize(int size)     { m_size = size; }
        void SetOffset(int offset) { m_offset = offset; }
        int  GetAllocSize()        { return m_size; }
        int  GetOffset()           { return m_offset; }

        virtual string AttributesToString() override
        {
            if (m_size == 0 && m_offset == 0) return "";
            string result = " size=\"" + std::to_string(m_size) + "\"";
            result += " offset=\"" + std::to_string(m_offset) + "\"";
            return result;
        }

        bool IsCompatibleWith(cDeclNode* other)
        {
            if (other == nullptr) return false;
            cDeclNode* thisType  = this->GetType();
            cDeclNode* otherType = other->GetType();
            if (thisType == nullptr || otherType == nullptr) return false;
            if (thisType == otherType) return true;

            int thisSize  = thisType->GetSize();
            int otherSize = otherType->GetSize();

            if (thisType->IsInt() && otherType->IsFloat()) return false;
            if (thisType->IsFloat() && otherType->IsInt()) return true;
            if (thisType->IsFloat() && otherType->IsFloat())
                return otherSize <= thisSize;
            if (thisType->IsInt() && otherType->IsInt())
                return otherSize <= thisSize;

            return false;
        }

    protected:
        int m_size;
        int m_offset;
};