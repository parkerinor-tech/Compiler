//**************************************
// cSymbolTable.cpp
//
// Implementation of symbol table
//
//Author: Parker Fagen
//**************************************
#include "cSymbolTable.h"

#include <unordered_map>
#include <vector>
#include <string>

using std::string;
using std::vector;

// Each scope is a hash table
typedef std::unordered_map<string, cSymbol *> Scope;

// Stack of scopes
static vector<Scope *> g_scopes;

//**************************************************
// Constructor
cSymbolTable::cSymbolTable()
{
    IncreaseScope(); // global scope
}

//**************************************************
symbolTable_t *cSymbolTable::IncreaseScope()
{
    Scope *scope = new Scope();
    g_scopes.push_back(scope);
    return nullptr;
}

//**************************************************
symbolTable_t *cSymbolTable::DecreaseScope()
{
    if (g_scopes.size() > 1)
    {
        g_scopes.pop_back();
    }
    return nullptr;
}

//**************************************************
void cSymbolTable::Insert(cSymbol *sym)
{
    Scope *current = g_scopes.back();
    (*current)[sym->GetName()] = sym;
}

//**************************************************
cSymbol *cSymbolTable::Find(string name)
{
    for (int i = g_scopes.size() - 1; i >= 0; i--)
    {
        auto it = g_scopes[i]->find(name);
        if (it != g_scopes[i]->end())
            return it->second;
    }
    return nullptr;
}

//**************************************************
cSymbol *cSymbolTable::FindLocal(string name)
{
    Scope *current = g_scopes.back();
    auto it = current->find(name);
    if (it != current->end())
        return it->second;
    return nullptr;
}
