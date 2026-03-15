#pragma once
//**************************************
// cComputeSize.h
//
// Visitor that computes sizes and offsets for all declarations
// and variable references.
//
#include "cVisitor.h"
#include "astnodes.h"
#include <string>
using std::string;

class cComputeSize : public cVisitor
{
public:
    static const int WORD_SIZE   = 4;
    static const int PARAM_START = -12;
    static const int PARAM_STEP  = -4;

    cComputeSize() : m_offset(0), m_highWater(0) {}

    virtual void VisitAllNodes(cAstNode* node) override { node->Visit(this); }

    //----------------------------------------------------------
    // Program: size = block size rounded up to word
    //----------------------------------------------------------
    virtual void Visit(cProgramNode* node) override
    {
        node->VisitAllChildren(this);
        cBlockNode* block = node->GetBlock();
        if (block != nullptr)
            node->SetSize(RoundUp(block->GetSize()));
    }

    //----------------------------------------------------------
    // Block: save/restore offset; size = high water mark for this block
    //----------------------------------------------------------
    virtual void Visit(cBlockNode* node) override
    {
        int savedOffset    = m_offset;
        int savedHighWater = m_highWater;
        m_highWater        = m_offset; // reset high water to block entry

        if (node->GetDecls() != nullptr)
            node->GetDecls()->Visit(this);
        if (node->GetStmts() != nullptr)
            node->GetStmts()->Visit(this);

        int blockSize = m_highWater - savedOffset;
        node->SetSize(blockSize);

        // Reclaim inner scope space; restore offset
        m_offset = savedOffset;
        // Keep high water if it grew (outer block needs to know)
        if (m_highWater < savedHighWater)
            m_highWater = savedHighWater;
    }

    //----------------------------------------------------------
    // VarDecl: compute size, align, assign offset
    //----------------------------------------------------------
    virtual void Visit(cVarDeclNode* node) override
    {
        int size = ComputeVarSize(node);
        node->SetSize(size);

        // Always word-align so pushvar/pushcvar offsets are valid
        m_offset = RoundUp(m_offset);

        node->SetOffset(m_offset);
        m_offset += size;

        if (m_offset > m_highWater)
            m_highWater = m_offset;
    }

    //----------------------------------------------------------
    // StructDecl: compute member offsets in a fresh scope
    //----------------------------------------------------------
    virtual void Visit(cStructDeclNode* node) override
    {
        int savedOffset    = m_offset;
        int savedHighWater = m_highWater;
        m_offset           = 0;
        m_highWater        = 0;

        cDeclsNode* decls = node->GetDecls();
        if (decls != nullptr) decls->Visit(this);

        node->SetSize(m_highWater);
        node->SetOffset(0);

        m_offset    = savedOffset;
        m_highWater = savedHighWater;
    }

    //----------------------------------------------------------
    // FuncDecl: params get negative offsets; locals start at 0
    //----------------------------------------------------------
    virtual void Visit(cFuncDeclNode* node) override
    {
        int savedOffset    = m_offset;
        int savedHighWater = m_highWater;

        // Process params
        cArgsNode* args = node->GetArgs();
        if (args != nullptr)
        {
            int paramOffset = PARAM_START;
            int argsSize    = 0;
            for (int i = 0; i < args->GetNumChildren(); i++)
            {
                cVarDeclNode* param = dynamic_cast<cVarDeclNode*>(args->GetChildNode(i));
                if (param == nullptr) continue;
                int psize = ComputeVarSize(param);
                param->SetSize(psize);
                param->SetOffset(paramOffset);
                paramOffset -= WORD_SIZE;
                argsSize    += WORD_SIZE;
            }
            args->SetSize(argsSize);
        }

        // Process locals
        m_offset    = 0;
        m_highWater = 0;

        cDeclsNode* decls = node->GetDecls();
        cStmtsNode* stmts = node->GetStmts();
        if (decls != nullptr) decls->Visit(this);
        if (stmts != nullptr) stmts->Visit(this);

        node->SetSize(RoundUp(m_highWater));
        node->SetOffset(0);

        m_offset    = savedOffset;
        m_highWater = savedHighWater;
    }

    //----------------------------------------------------------
    // VarExprNode: copy size/offset from decl; handle arrays/structs
    //----------------------------------------------------------
    virtual void Visit(cVarExprNode* node) override
    {
        node->VisitAllChildren(this);

        cSymbol* sym = node->GetName();
        if (sym == nullptr || sym->GetDecl() == nullptr) return;

        cDeclNode* decl = sym->GetDecl();
        int numExtra = node->GetNumChildren() - 1; // children beyond the base symbol

        if (numExtra == 0)
        {
            // Plain varref
            node->SetSize(decl->GetAllocSize());
            node->SetOffset(decl->GetOffset());
        }
        else
        {
            // Check if extra children are symbols (struct) or exprs (array)
            bool hasExpr = false;
            for (int i = 1; i <= numExtra; i++)
            {
                if (dynamic_cast<cSymbol*>(node->GetChildNode(i)) == nullptr)
                { hasExpr = true; break; }
            }

            if (hasExpr)
                ComputeArrayRef(node, decl);
            else
                ComputeStructRef(node, decl);
        }
    }

    //----------------------------------------------------------
    // ParamsNode: sum of actual param sizes (word-aligned each)
    //----------------------------------------------------------
    virtual void Visit(cParamsNode* node) override
    {
        node->VisitAllChildren(this);
        int total = 0;
        for (int i = 0; i < node->GetNumChildren(); i++)
        {
            cExprNode* p = dynamic_cast<cExprNode*>(node->GetChildNode(i));
            if (p != nullptr && p->GetType() != nullptr)
                total += RoundUp1(p->GetType()->GetSize());
        }
        node->SetSize(total);
    }

    //----------------------------------------------------------
    // ArrayDecl: allocate space for the array variable
    //----------------------------------------------------------
    virtual void Visit(cArrayDeclNode* node) override
    {
        // Compute total size: count * element_size
        int elemSize = 0;
        cSymbol* elemSym = node->GetElemTypeSym();
        if (elemSym != nullptr && elemSym->GetDecl() != nullptr)
            elemSize = ComputeDeclSize(elemSym->GetDecl());
        int totalSize = node->GetCount() * elemSize;
        node->SetSize(totalSize);

        // Arrays always word-aligned
        m_offset = RoundUp(m_offset);
        node->SetOffset(m_offset);
        m_offset += totalSize;

        if (m_offset > m_highWater)
            m_highWater = m_offset;
    }

    //----------------------------------------------------------
    // DeclsNode: visit children and record total size
    //----------------------------------------------------------
    virtual void Visit(cDeclsNode* node) override
    {
        int before = m_offset;
        node->VisitAllChildren(this);
        node->SetSize(m_offset - before);
    }

    //----------------------------------------------------------
    // Default: just visit children
    //----------------------------------------------------------
    virtual void Visit(cAstNode*        node) override { node->VisitAllChildren(this); }
    virtual void Visit(cDeclNode*       node) override { node->VisitAllChildren(this); }
    virtual void Visit(cStmtNode*       node) override { node->VisitAllChildren(this); }
    virtual void Visit(cStmtsNode*      node) override { node->VisitAllChildren(this); }
    virtual void Visit(cAssignNode*     node) override { node->VisitAllChildren(this); }
    virtual void Visit(cExprNode*       node) override { node->VisitAllChildren(this); }
    virtual void Visit(cBinaryExprNode* node) override { node->VisitAllChildren(this); }
    virtual void Visit(cUnaryExprNode*  node) override { node->VisitAllChildren(this); }
    virtual void Visit(cIntExprNode*    node) override { node->VisitAllChildren(this); }
    virtual void Visit(cFloatExprNode*  node) override { node->VisitAllChildren(this); }
    virtual void Visit(cPrintNode*      node) override { node->VisitAllChildren(this); }
    virtual void Visit(cPrintsNode*     node) override { node->VisitAllChildren(this); }
    virtual void Visit(cIfNode*         node) override { node->VisitAllChildren(this); }
    virtual void Visit(cWhileNode*      node) override { node->VisitAllChildren(this); }
    virtual void Visit(cReturnNode*     node) override { node->VisitAllChildren(this); }
    virtual void Visit(cFuncCallNode* node) override
    {
        node->VisitAllChildren(this);

        // params size = number of params * WORD_SIZE
        cParamsNode* params = node->GetParams();
        if (params != nullptr)
        {
            int numParams = params->GetNumChildren();
            params->SetSize(numParams * WORD_SIZE);
        }
    }
    virtual void Visit(cOpNode*         node) override { node->VisitAllChildren(this); }
    virtual void Visit(cSymbol*         node) override { node->VisitAllChildren(this); }
    virtual void Visit(cBaseTypeNode*   node) override { node->VisitAllChildren(this); }
    virtual void Visit(cFuncHeaderNode* node) override { node->VisitAllChildren(this); }
    virtual void Visit(cArgsNode*       node) override { node->VisitAllChildren(this); }

private:
    int m_offset;
    int m_highWater;

    static int RoundUp(int n)
    {
        return (n + WORD_SIZE - 1) & ~(WORD_SIZE - 1);
    }

    static int RoundUp1(int n)  // only round if > 1
    {
        return (n > 1) ? RoundUp(n) : n;
    }

    // Compute the total allocated size of a variable (handles array/struct types)
    int ComputeVarSize(cVarDeclNode* node)
    {
        cSymbol* typeSym = node->GetTypeSymbol();
        if (typeSym == nullptr || typeSym->GetDecl() == nullptr) return 0;
        cDeclNode* typeDecl = typeSym->GetDecl();
        return ComputeDeclSize(typeDecl);
    }

    // Compute size of a type decl (recurses for arrays)
    int ComputeDeclSize(cDeclNode* typeDecl)
    {
        if (typeDecl->IsArray())
        {
            cArrayDeclNode* ad = dynamic_cast<cArrayDeclNode*>(typeDecl);
            if (ad == nullptr) return 0;
            cSymbol* elemSym = ad->GetElemTypeSym();
            if (elemSym == nullptr || elemSym->GetDecl() == nullptr) return 0;
            return ad->GetCount() * ComputeDeclSize(elemSym->GetDecl());
        }
        else if (typeDecl->IsStruct())
        {
            return typeDecl->GetAllocSize();
        }
        else
        {
            return typeDecl->GetSize();
        }
    }

    // Element size of an array type (for rowsizes)
    int ComputeElementSize(cDeclNode* arrayDecl)
    {
        cArrayDeclNode* ad = dynamic_cast<cArrayDeclNode*>(arrayDecl);
        if (ad == nullptr) return 0;
        cSymbol* elemSym = ad->GetElemTypeSym();
        if (elemSym == nullptr || elemSym->GetDecl() == nullptr) return 0;
        return ComputeDeclSize(elemSym->GetDecl());
    }

    void ComputeArrayRef(cVarExprNode* node, cDeclNode* decl)
    {
        node->SetSize(decl->GetAllocSize());
        node->SetOffset(decl->GetOffset());

        // Build rowsizes by descending through array type dimensions
        cSymbol* typeSym = dynamic_cast<cVarDeclNode*>(decl) ? dynamic_cast<cVarDeclNode*>(decl)->GetTypeSymbol() : nullptr;
        if (typeSym == nullptr || typeSym->GetDecl() == nullptr) return;

        cDeclNode* currentType = typeSym->GetDecl();
        string rowSizes = "";

        for (int i = 1; i < node->GetNumChildren(); i++)
        {
            if (dynamic_cast<cSymbol*>(node->GetChildNode(i)) != nullptr)
                continue; // skip struct refs

            int elemSize = ComputeElementSize(currentType);
            if (!rowSizes.empty()) rowSizes += " ";
            rowSizes += std::to_string(elemSize);

            // Descend into element type
            cArrayDeclNode* ad = dynamic_cast<cArrayDeclNode*>(currentType);
            if (ad != nullptr)
            {
                cSymbol* es = ad->GetElemTypeSym();
                if (es != nullptr && es->GetDecl() != nullptr)
                    currentType = es->GetDecl();
            }
        }
        node->SetRowSizes(rowSizes);
    }

    void ComputeStructRef(cVarExprNode* node, cDeclNode* decl)
    {
        int totalOffset = decl->GetOffset();
        int finalSize   = decl->GetAllocSize();

        for (int i = 1; i < node->GetNumChildren(); i++)
        {
            cSymbol* memberSym = dynamic_cast<cSymbol*>(node->GetChildNode(i));
            if (memberSym == nullptr || memberSym->GetDecl() == nullptr) continue;

            cDeclNode* memberDecl = memberSym->GetDecl();
            totalOffset += memberDecl->GetOffset();
            finalSize    = memberDecl->GetAllocSize();
        }

        node->SetSize(finalSize);
        node->SetOffset(totalOffset);
    }
};