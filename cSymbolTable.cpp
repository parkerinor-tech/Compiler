//************************************************************
//cSymbolTable.cpp
//
//Implementaion of symbol table
//
//Author: Parker Fagen
//
#include "cSymbolTable.h"
#include <vector>
#include <unordered_map>

using std::string;
using std:vector;
using std:: unorderd_map;

typedef unorderd_map<string, cSymbol *> Scope;

static vector<Scope *> g_scopes;

//**********************************************************
//Construct an empty symbol table
cSymbolTable::cSymbolTable()
{
    IncreaseScope();
}

//************************************************************
//Increase the scope
cSymbolTable_t *cSymbolTable::IncreaseScope()
{
    Scope *scope - new Scope();
    g_scopes.push_back(scope);
    return scope;
}
