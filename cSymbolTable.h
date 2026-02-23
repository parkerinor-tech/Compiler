#pragma once
//****************************************************
// Starter code for a symbol table definition.
//
#include <unordered_map>
#include <vector>
#include <string>
#include "cSymbol.h"

typedef std::unordered_map<std::string, cSymbol*> symbolTable_t;

class cSymbolTable
{
    public:
        // Construct an empty symbol table
        cSymbolTable();

        // Increase the scope: add a level to the nested symbol table
        // Return value is the newly created scope
        symbolTable_t *IncreaseScope();

        // Decrease the scope: remove the outer-most scope.
        // Returned value is the outer-most scope AFTER the pop.
        //
        // NOTE: do NOT clean up memory after poping the table. Parts of the
        // AST will probably contain pointers to symbols in the popped table.
        symbolTable_t *DecreaseScope();

        // insert a symbol into the table
        // Assumes the symbol is not already in the table
        void Insert(cSymbol *sym);

        // Do a lookup in the nested table. 
        // NOTE: This starts at the inner-most scope and works its way out until
        // a match is found.
        // Return the symbol for the inner-most match. 
        // Returns nullptr if no match is found.
        cSymbol *Find(std::string name);

        // Find a symbol in the inner-most scope.
        // NOTE: This ONLY searches the inner-most scope.
        // Returns nullptr if the symbol is not found.
        cSymbol *FindLocal(std::string name);

        // Return the current (innermost) scope directly
        symbolTable_t *GetCurrentScope() { return mScopes.back(); }

        // Populate root scope with built-in types (char, int, float, long, double)
        void InitRootTable();

        // Return the parent (second innermost) scope, or null if only one scope
        symbolTable_t *GetParentScope()
        {
            if (mScopes.size() < 2) return nullptr;
            return mScopes[mScopes.size() - 2];
        }

        // Find a symbol in the parent scope only
        cSymbol *FindInParent(std::string name)
        {
            symbolTable_t *parent = GetParentScope();
            if (parent == nullptr) return nullptr;
            auto found = parent->find(name);
            if (found != parent->end()) return found->second;
            return nullptr;
        }

    private:
        // Stack of scopes (innermost scope is back())
        std::vector<symbolTable_t*> mScopes;
};

// declare the global symbol table. The definition will have to be in a cpp file
extern cSymbolTable g_symbolTable;
// Alias for compatibility with new main.cpp
#define g_SymbolTable g_symbolTable