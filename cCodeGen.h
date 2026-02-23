#pragma once
// cCodeGen.h - stub for Lab 7
#include "cVisitor.h"
#include "astnodes.h"
#include <string>
using std::string;

class cCodeGen : public cVisitor
{
public:
    cCodeGen(string filename) {}
    virtual void VisitAllNodes(cAstNode* node) override { node->Visit(this); }
};
