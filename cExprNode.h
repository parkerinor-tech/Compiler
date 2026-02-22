#pragma once
//**************************************
// cExprNode.h
//
// Defines base class for all expressions
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cStmtNode.h"

// Forward declaration
class cDeclNode;

class cExprNode : public cStmtNode
{
    public:
        cExprNode() : cStmtNode() {}

        // Every expression must be able to report its type
        virtual cDeclNode* GetType() = 0;
};