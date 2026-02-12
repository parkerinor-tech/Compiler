// cSymbolTable.cpp
#include "cSymbolTable.h"
#include <iostream> // optional, only if you want to debug

cSymbolTable::cSymbolTable()
{
    // Create global scope
    IncreaseScope();
}

symbolTable_t *cSymbolTable::IncreaseScope()
{
    symbolTable_t *scope = new symbolTable_t();
    mScopes.push_back(scope);
    return scope;
}

symbolTable_t *cSymbolTable::DecreaseScope()
{
    // Never remove the global scope
    if (mScopes.size() > 1)
        mScopes.pop_back();

    return mScopes.back();
}

void cSymbolTable::Insert(cSymbol *sym)
{
    // back() returns symbolTable_t*
    (*mScopes.back())[sym->GetName()] = sym;
}

cSymbol *cSymbolTable::Find(string name)
{
    // Search from innermost scope outward
    for (auto it = mScopes.rbegin(); it != mScopes.rend(); ++it)
    {
        symbolTable_t *table = *it;

        auto found = table->find(name);
        if (found != table->end())
            return found->second;
    }
    return nullptr;
}

cSymbol *cSymbolTable::FindLocal(string name)
{
    symbolTable_t *current = mScopes.back();

    auto found = current->find(name);
    if (found != current->end())
        return found->second;

    return nullptr;
}// Constructor: start with global scope
