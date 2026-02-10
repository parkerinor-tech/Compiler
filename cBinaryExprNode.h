#pragma once
//**************************************
// cBinaryExprNode.h
//
// AST node for a binary expression like:
//      a + b
//      x * y
//      5 - 3
//
// Inherits from cExprNode so it can be used anywhere
// expressions are valid.
//
// Author: Parker Fagen
//

#include "cExprNode.h"
#include "cOpNode.h"

class cBinaryExprNode : public cExprNode
{
    public:
        // left  = left expression
        // op    = operator node (+, -, *, /, etc.)
        // right = right expression
        cBinaryExprNode(cExprNode *left, cOpNode *op, cExprNode *right)
            : cExprNode()
        {
            AddChild(left);
            AddChild(op);
            AddChild(right);
        }

        virtual string NodeType() { return string("expr"); }

        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
