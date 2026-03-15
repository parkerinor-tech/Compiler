#pragma once
//**************************************
// cCodeGen.h
//
// Code generation visitor. Emits stackl assembly via EmitString/EmitInt.
//
#include "cVisitor.h"
#include "astnodes.h"
#include "emit.h"
#include <string>
#include <sstream>
#include <vector>
using std::string;

class cCodeGen : public cVisitor
{
public:
    cCodeGen(string filename)
    {
        InitOutput(filename);
    }

    ~cCodeGen()
    {
        FinalizeOutput();
    }

    virtual void VisitAllNodes(cAstNode* node) override
    {
        node->Visit(this);
    }

    //----------------------------------------------------------
    // Program
    //----------------------------------------------------------
    virtual void Visit(cProgramNode* node) override
    {
        int globalSize = node->GetSize();
        // Emit function definitions BEFORE main label
        cBlockNode* block = node->GetBlock();
        if (block->GetDecls() != nullptr)
            block->GetDecls()->Visit(this);
        // Main program entry point
        EmitString("main:\n");
        if (globalSize > 0)
            EmitString("adjsp " + Str(globalSize) + "\n");
        if (block->GetStmts() != nullptr)
            block->GetStmts()->Visit(this);
        EmitString("halt\n");
    }

    //----------------------------------------------------------
    // Block
    //----------------------------------------------------------
    virtual void Visit(cBlockNode* node) override
    {
        if (node->GetDecls() != nullptr) node->GetDecls()->Visit(this);
        if (node->GetStmts() != nullptr) node->GetStmts()->Visit(this);
    }

    //----------------------------------------------------------
    // Decls
    //----------------------------------------------------------
    virtual void Visit(cDeclsNode* node) override
    {
        node->VisitAllChildren(this);
    }

    //----------------------------------------------------------
    // VarDecl: nothing at codegen time
    //----------------------------------------------------------
    virtual void Visit(cVarDeclNode* node) override { }

    //----------------------------------------------------------
    // FuncDecl: emit only definitions
    //----------------------------------------------------------
    virtual void Visit(cFuncDeclNode* node) override
    {
        if (!node->HasBody()) return;

        string name = node->GetName();
        EmitString("\n.function " + name + "\n");
        EmitString(name + ":\n");

        cDeclsNode* decls = node->GetDecls();
        int localSize = (decls != nullptr) ? RoundUp(decls->GetSize()) : 0;
        if (localSize > 0)
            EmitString("adjsp " + Str(localSize) + "\n");

        if (decls  != nullptr) decls->Visit(this);
        cStmtsNode* stmts = node->GetStmts();
        if (stmts != nullptr) stmts->Visit(this);

        // Safety return
        EmitString("push 0\n");
        EmitString("returnv\n");
    }

    //----------------------------------------------------------
    // Assign: addr of lhs, value of rhs, store
    //----------------------------------------------------------
    virtual void Visit(cAssignNode* node) override
    {
        EmitString(";;; assign\n");
        // POPVARIND pops address first (top of stack), then value
        // So push value first, then address
        node->GetRhs()->Visit(this);
        EmitVarAddr(node->GetLhs());
        if (IsCharVar(node->GetLhs()))
            EmitString("popcvarind\n");
        else
            EmitString("popvarind\n");
        // popvarind/popcvarind consumes both addr and value - no extra pop needed
    }

    //----------------------------------------------------------
    // BinaryExpr: lhs op rhs
    //----------------------------------------------------------
    virtual void Visit(cBinaryExprNode* node) override
    {
        // BinaryExpr children: 0=lhs, 1=op, 2=rhs
        cExprNode* lhs = dynamic_cast<cExprNode*>(node->GetLhs());
        cOpNode*   op  = node->GetOp();
        cExprNode* rhs = dynamic_cast<cExprNode*>(node->GetRhs());

        lhs->Visit(this);
        rhs->Visit(this);
        EmitBinOp(op->GetOp());
    }

    //----------------------------------------------------------
    // UnaryExpr: negate
    //----------------------------------------------------------
    virtual void Visit(cUnaryExprNode* node) override
    {
        node->GetExpr()->Visit(this);
        EmitString("push 0\n");
        EmitString("swap\n");
        EmitString("minus\n");  // 0 - x = -x
    }

    //----------------------------------------------------------
    // IntExpr: push literal
    //----------------------------------------------------------
    virtual void Visit(cIntExprNode* node) override
    {
        string attr = node->AttributesToString();
        // attr = " value=\"N\""
        size_t q1 = attr.find('"');
        size_t q2 = attr.rfind('"');
        string val = attr.substr(q1 + 1, q2 - q1 - 1);
        EmitString("push " + val + "\n");
    }

    //----------------------------------------------------------
    // VarExpr: push value
    //----------------------------------------------------------
    virtual void Visit(cVarExprNode* node) override
    {
        EmitString(";;; varref\n");
        cSymbol* sym = node->GetName();
        if (sym == nullptr || sym->GetDecl() == nullptr) return;

        if (!node->IsArray())
        {
            // Simple variable
            int offset = sym->GetDecl()->GetOffset();
            if (IsCharExpr(node))
                EmitString("pushcvar " + Str(offset) + "\n");
            else
                EmitString("pushvar " + Str(offset) + "\n");
        }
        else
        {
            // Array element: compute address then fetch
            EmitArrayAddr(node);
            if (IsCharExpr(node))
                EmitString("pushcvarind\n");
            else
                EmitString("pushvarind\n");
        }
    }

    //----------------------------------------------------------
    // Print: push expr, call print, discard
    //----------------------------------------------------------
    virtual void Visit(cPrintNode* node) override
    {
        EmitString(";;; print\n");
        node->GetExpr()->Visit(this);
        EmitString("call @print\n");
        EmitString("swap\npop\n");  // remove arg
        EmitString("pop\n");        // remove return value
    }

    //----------------------------------------------------------
    // FuncCall: push args, call, clean stack
    //----------------------------------------------------------
    virtual void Visit(cFuncCallNode* node) override
    {
        EmitString(";;; funcCall " + node->GetName()->GetName() + "\n");
        cParamsNode* params = node->GetParams();
        int numParams = 0;

        if (params != nullptr)
        {
            numParams = params->GetNumChildren();
            for (int i = 0; i < numParams; i++)
            {
                cExprNode* expr = dynamic_cast<cExprNode*>(params->GetParam(i));
                if (expr != nullptr) expr->Visit(this);
            }
        }

        EmitString("call @" + node->GetName()->GetName() + "\n");

        // Clean up args (SWAP past return value, POP each)
        for (int i = 0; i < numParams; i++)
            EmitString("swap\npop\n");
        // Return value now on top
    }

    //----------------------------------------------------------
    // Return
    //----------------------------------------------------------
    virtual void Visit(cReturnNode* node) override
    {
        EmitString(";;; return\n");
        node->GetExpr()->Visit(this);
        EmitString("returnv\n");
    }

    //----------------------------------------------------------
    // If
    //----------------------------------------------------------
    virtual void Visit(cIfNode* node) override
    {
        EmitString(";;; if\n");
        string elseLabel = GenerateLabel();
        string endLabel  = GenerateLabel();

        bool hasElse = (node->GetElse() != nullptr);

        node->GetCond()->Visit(this);
        EmitString("jumpe @" + (hasElse ? elseLabel : endLabel) + "\n");

        node->GetIfPart()->Visit(this);

        if (hasElse)
        {
            EmitString("jump @" + endLabel + "\n");
            EmitString(elseLabel + ":\n");
            node->GetElse()->Visit(this);
        }

        EmitString(endLabel + ":\n");
    }

    //----------------------------------------------------------
    // While
    //----------------------------------------------------------
    virtual void Visit(cWhileNode* node) override
    {
        EmitString(";;; while\n");
        string topLabel = GenerateLabel();
        string endLabel = GenerateLabel();

        EmitString(topLabel + ":\n");
        node->GetCond()->Visit(this);
        EmitString("jumpe @" + endLabel + "\n");
        node->GetBody()->Visit(this);
        EmitString("jump @" + topLabel + "\n");
        EmitString(endLabel + ":\n");
    }

    //----------------------------------------------------------
    // Default visitors
    //----------------------------------------------------------
    virtual void Visit(cStmtsNode*      node) override { node->VisitAllChildren(this); }
    virtual void Visit(cAstNode*        node) override { node->VisitAllChildren(this); }
    virtual void Visit(cStmtNode*       node) override { node->VisitAllChildren(this); }
    virtual void Visit(cExprNode*       node) override { node->VisitAllChildren(this); }
    virtual void Visit(cDeclNode*       node) override { }
    virtual void Visit(cArrayDeclNode*  node) override { }
    virtual void Visit(cStructDeclNode* node) override { }
    virtual void Visit(cBaseTypeNode*   node) override { }
    virtual void Visit(cSymbol*         node) override { }
    virtual void Visit(cOpNode*         node) override { }
    virtual void Visit(cFuncHeaderNode* node) override { }
    virtual void Visit(cArgsNode*       node) override { }
    virtual void Visit(cParamsNode*     node) override { node->VisitAllChildren(this); }
    virtual void Visit(cFloatExprNode*  node) override { }
    virtual void Visit(cPrintsNode*     node) override { node->VisitAllChildren(this); }

private:
    static string Str(int n) { return std::to_string(n); }
    static int RoundUp(int n) { return (n + 3) & ~3; }

    bool IsCharExpr(cExprNode* node)
    {
        if (node == nullptr) return false;
        cDeclNode* type = node->GetType();
        return (type != nullptr && type->IsChar());
    }

    bool IsCharVar(cVarExprNode* node) { return IsCharExpr(node); }

    //----------------------------------------------------------
    // Push ADDRESS of a variable: PUSH offset PUSHFP PLUS
    //----------------------------------------------------------
    void EmitVarAddr(cVarExprNode* node)
    {
        cSymbol* sym = node->GetName();
        if (sym == nullptr || sym->GetDecl() == nullptr) return;

        if (!node->IsArray())
        {
            int offset = sym->GetDecl()->GetOffset();
            EmitString("push " + Str(offset) + "\n");
            EmitString("pushfp\n");
            EmitString("plus\n");
        }
        else
        {
            EmitArrayAddr(node);
        }
    }

    //----------------------------------------------------------
    // Push address of array element: base + index * elemSize
    //----------------------------------------------------------
    void EmitArrayAddr(cVarExprNode* node)
    {
        cSymbol* sym = node->GetName();
        if (sym == nullptr || sym->GetDecl() == nullptr) return;
        int baseOffset = sym->GetDecl()->GetOffset();

        // Push base address
        EmitString("push " + Str(baseOffset) + "\n");
        EmitString("pushfp\n");
        EmitString("plus\n");

        // Get row sizes from cComputeSize
        string rowSizesStr = node->GetRowSizes();
        std::istringstream ss(rowSizesStr);
        std::vector<int> rowSizes;
        int v;
        while (ss >> v) rowSizes.push_back(v);

        int dimIdx = 0;
        for (int i = 0; i < node->GetNumSubscripts(); i++)
        {
            cExprNode* idx = node->GetSubscript(i);
            if (idx == nullptr) continue;

            idx->Visit(this);
            int elemSize = (dimIdx < (int)rowSizes.size()) ? rowSizes[dimIdx] : 4;
            EmitString("push " + Str(elemSize) + "\n");
            EmitString("times\n");
            EmitString("plus\n");
            dimIdx++;
        }
    }

    //----------------------------------------------------------
    // Binary operator
    //----------------------------------------------------------
    // Emit logical NOT: 0->1, nonzero->0
    void EmitLogicalNot()
    {
        string tLabel = GenerateLabel();
        string eLabel = GenerateLabel();
        EmitString("jumpe @" + tLabel + "\n");  // if 0, jump to push 1
        EmitString("push 0\n");
        EmitString("jump @" + eLabel + "\n");
        EmitString(tLabel + ":\n");
        EmitString("push 1\n");
        EmitString(eLabel + ":\n");
    }

    void EmitBinOp(int op)
    {
        switch (op)
        {
            case '+':  EmitString("plus\n");   break;
            case '-':  EmitString("minus\n");  break;
            case '*':  EmitString("times\n");  break;
            case '/':  EmitString("divide\n"); break;
            case '%':
                // a%b: stack has [a, b]. Save b, compute a-(a/b)*b
                EmitString("popreg 0\n");   // reg=b; stack: a
                EmitString("dup\n");      // a a
                EmitString("pushreg 0\n");  // a a b
                EmitString("divide\n");   // a (a/b)
                EmitString("pushreg 0\n");  // a (a/b) b
                EmitString("times\n");    // a (a/b)*b
                EmitString("minus\n");    // a%b
                break;
            case '<':        EmitString("LT\n");       break;  // LT: second<top = a<b ✓
            case '>':        EmitString("swap\n"); EmitString("LT\n"); break; // swap then LT: a<b with swapped = b<a = a>b
            case EQUALS:     EmitString("EQ\n");       break;
            case LE:
                // a<=b = NOT(a>b) = NOT(swap+LT)
                EmitString("swap\n");
                EmitString("LT\n");
                EmitLogicalNot();
                break;
            case GE:
                // a>=b = NOT(a<b) = NOT(LT)
                EmitString("LT\n");
                EmitLogicalNot();
                break;
            case NOT_EQUALS:
                EmitString("EQ\n");
                EmitLogicalNot();
                break;
            case AND:  EmitString("band\n"); break;
            case OR:
                // a OR b = NOT(NOT a AND NOT b) -- de Morgan
                // OR: if either nonzero -> 1
                // Simple: a b band -> 0 only if both 0... NO, band is bitwise
                // Use: a b plus, then check nonzero
                EmitString("plus\n");
                {
                    string tLabel = GenerateLabel();
                    string eLabel = GenerateLabel();
                    EmitString("jumpe @" + tLabel + "\n");
                    EmitString("push 1\n");
                    EmitString("jump @" + eLabel + "\n");
                    EmitString(tLabel + ":\n");
                    EmitString("push 0\n");
                    EmitString(eLabel + ":\n");
                }
                break;
            default:   EmitString(";;; unknown op\n"); break;
        }
    }
};