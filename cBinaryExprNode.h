#pragma once
#include "cExprNode.h"
#include "cOpNode.h"
#include "cSymbolTable.h"

class cBinaryExprNode : public cExprNode
{
public:
    cBinaryExprNode(cExprNode* lhs, cOpNode* op, cExprNode* rhs)
    {
        AddChild(lhs);
        AddChild(op);
        AddChild(rhs);
    }

    virtual cDeclNode* GetType() override
    {
        cOpNode* op = dynamic_cast<cOpNode*>(GetChild(1));
        if (op != nullptr)
        {
            int opVal = op->GetOp();
            // Relational and logical operators always return int
            if (opVal == '>' || opVal == '<' ||
                opVal == GE  || opVal == LE  ||
                opVal == EQUALS || opVal == NOT_EQUALS ||
                opVal == AND || opVal == OR)
            {
                cSymbol* sym = g_symbolTable.Find("int");
                if (sym != nullptr) return sym->GetDecl();
                return nullptr;
            }
        }

        // Arithmetic: return the "larger" type
        cExprNode* lhs = dynamic_cast<cExprNode*>(GetChild(0));
        cExprNode* rhs = dynamic_cast<cExprNode*>(GetChild(2));
        if (lhs == nullptr) return nullptr;
        if (rhs == nullptr) return lhs->GetType();

        cDeclNode* lhsType = lhs->GetType();
        cDeclNode* rhsType = rhs->GetType();
        if (lhsType == nullptr) return rhsType;
        if (rhsType == nullptr) return lhsType;

        // Float beats int
        if (lhsType->IsFloat() && rhsType->IsInt()) return lhsType;
        if (rhsType->IsFloat() && lhsType->IsInt()) return rhsType;

        // Same category: larger size wins
        if (lhsType->GetSize() >= rhsType->GetSize()) return lhsType;
        return rhsType;
    }

    virtual string NodeType() { return "expr"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};