#pragma once
//**************************************
// cSemantics.h
//
// Visitor subclass that performs semantic error checking for Lab 5B.
//

#include "cVisitor.h"
#include "astnodes.h"

class cSemantics : public cVisitor
{
public:
    cSemantics() {}

    virtual void VisitAllNodes(cAstNode* node) override
    {
        node->Visit(this);
    }

    //----------------------------------------------------------
    // cAssignNode: check that rhs type is compatible with lhs type
    //----------------------------------------------------------
    virtual void Visit(cAssignNode* node) override
    {
        node->VisitAllChildren(this);

        cVarExprNode* lhs = node->GetLhs();
        cExprNode*    rhs = node->GetRhs();

        if (lhs == nullptr || rhs == nullptr) return;

        cDeclNode* lhsType = lhs->GetType();
        cDeclNode* rhsType = rhs->GetType();

        if (lhsType == nullptr || rhsType == nullptr) return;

        if (!lhsType->IsCompatibleWith(rhsType))
        {
            node->SemanticError("Cannot assign " + rhsType->GetName() +
                                " to " + lhsType->GetName());
        }
    }

    //----------------------------------------------------------
    // cVarExprNode: catch function names used as variables;
    //              check array subscript validity
    //----------------------------------------------------------
    virtual void Visit(cVarExprNode* node) override
    {
        node->VisitAllChildren(this);

        cSymbol* sym = node->GetName();
        if (sym == nullptr) return;

        cDeclNode* decl = sym->GetDecl();
        if (decl == nullptr) return;

        // Check if the base symbol is a function used as a variable
        if (decl->IsFunc())
        {
            node->SemanticError("Symbol " + sym->GetName() +
                                " is a function, not a variable");
            return;
        }

        // Check array subscript children (children[1..n] are subscript exprs)
        for (int i = 1; i < node->GetNumChildren(); i++)
        {
            cExprNode* idx = dynamic_cast<cExprNode*>(node->GetChildNode(i));
            if (idx == nullptr) continue;  // struct ref - ignore per spec

            // decl->IsArray() handles both cArrayDeclNode directly
            // and cVarDeclNode whose type is an array
            if (!decl->IsArray())
            {
                node->SemanticError(sym->GetName() + " is not an array");
                return;
            }

            // Index must be an integer type
            cDeclNode* idxType = idx->GetType();
            if (idxType != nullptr && !idxType->IsInt())
            {
                node->SemanticError("Index of " + sym->GetName() +
                                    " is not an int");
            }
        }
    }

    //----------------------------------------------------------
    // cFuncCallNode: check arg count, arg types, body defined
    //----------------------------------------------------------
    virtual void Visit(cFuncCallNode* node) override
    {
        node->VisitAllChildren(this);

        cSymbol* nameSym = node->GetName();
        if (nameSym == nullptr) return;

        cDeclNode* decl = nameSym->GetDecl();
        if (decl == nullptr || !decl->IsFunc()) return;

        cFuncDeclNode* funcDecl = dynamic_cast<cFuncDeclNode*>(decl);
        if (funcDecl == nullptr) return;

        string funcName = nameSym->GetName();

        // Check function has a body (fully defined)
        if (!funcDecl->HasBody())
        {
            node->SemanticError("Function " + funcName + " is not fully defined");
            return;
        }

        // Get formal args and actual params
        cArgsNode*   args   = funcDecl->GetArgs();
        cParamsNode* params = node->GetParams();

        int argCount   = (args   != nullptr) ? args->GetNumChildren()   : 0;
        int paramCount = (params != nullptr) ? params->GetNumChildren() : 0;

        if (argCount != paramCount)
        {
            node->SemanticError(funcName + " called with wrong number of arguments");
            return;
        }

        // Check each argument type is compatible with formal parameter type
        for (int i = 0; i < argCount; i++)
        {
            cDeclNode* formalDecl = dynamic_cast<cDeclNode*>(args->GetChildNode(i));
            cExprNode* actualExpr = dynamic_cast<cExprNode*>(params->GetChildNode(i));

            if (formalDecl == nullptr || actualExpr == nullptr) continue;

            cDeclNode* formalType = formalDecl->GetType();
            cDeclNode* actualType = actualExpr->GetType();

            if (formalType == nullptr || actualType == nullptr) continue;

            if (!formalType->IsCompatibleWith(actualType))
            {
                node->SemanticError(funcName + " called with incompatible argument");
            }
        }
    }

    //----------------------------------------------------------
    // Default: visit all children
    //----------------------------------------------------------
    virtual void Visit(cAstNode*       node) override { node->VisitAllChildren(this); }
    virtual void Visit(cBlockNode*     node) override { node->VisitAllChildren(this); }
    virtual void Visit(cDeclNode*      node) override { node->VisitAllChildren(this); }
    virtual void Visit(cDeclsNode*     node) override { node->VisitAllChildren(this); }
    virtual void Visit(cExprNode*      node) override { node->VisitAllChildren(this); }
    virtual void Visit(cIntExprNode*   node) override { node->VisitAllChildren(this); }
    virtual void Visit(cFloatExprNode* node) override { node->VisitAllChildren(this); }
    virtual void Visit(cBinaryExprNode*node) override { node->VisitAllChildren(this); }
    virtual void Visit(cUnaryExprNode* node) override { node->VisitAllChildren(this); }
    virtual void Visit(cOpNode*        node) override { node->VisitAllChildren(this); }
    virtual void Visit(cPrintNode*     node) override { node->VisitAllChildren(this); }
    virtual void Visit(cProgramNode*   node) override { node->VisitAllChildren(this); }
    virtual void Visit(cStmtNode*      node) override { node->VisitAllChildren(this); }
    virtual void Visit(cStmtsNode*     node) override { node->VisitAllChildren(this); }
    virtual void Visit(cSymbol*        node) override { node->VisitAllChildren(this); }
    virtual void Visit(cArgsNode*      node) override { node->VisitAllChildren(this); }
    virtual void Visit(cParamsNode*    node) override { node->VisitAllChildren(this); }
    virtual void Visit(cFuncHeaderNode*node) override { node->VisitAllChildren(this); }
    virtual void Visit(cFuncDeclNode*  node) override { node->VisitAllChildren(this); }
    virtual void Visit(cReturnNode*    node) override { node->VisitAllChildren(this); }
    virtual void Visit(cIfNode*        node) override { node->VisitAllChildren(this); }
    virtual void Visit(cWhileNode*     node) override { node->VisitAllChildren(this); }
};