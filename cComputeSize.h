#pragma once
// cComputeSize.h - stub for Lab 6
#include "cVisitor.h"
#include "astnodes.h"

class cComputeSize : public cVisitor
{
public:
    cComputeSize() {}
    virtual void VisitAllNodes(cAstNode* node) override { node->Visit(this); }
};
