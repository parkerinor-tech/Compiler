%{
//**************************************
// lang.l
//
// scanner definition file. flex uses this file to create the scanner
//
// Author: <<<< insert your name here >>>>
//

#include "cSymbol.h"
#include "cSymbolTable.h"
#include "lex.h"
#include "tokens.h"
#include <stdio.h>
#include <string.h>

extern int g_insert;
extern cSymbolTable g_symbolTable;
// Macros that can be used to create debug output from the scanner
// Uncomment the next line of code to generate debug output.
// Note: for this to work, use DO_RETURN(val) for all the return statements
//       for your tokens. See sample below for "program"
#define DEBUG_OUTPUT
#ifdef DEBUG_OUTPUT
    #define DO_RETURN(a) { return Return(a); }
#else
    #define DO_RETURN(a) { return (a); }
#endif

int Return(int val);
static int ProcessIdentifier(char * inputIdentifier);
%}

%option noyywrap
%option noinput
%option nounput
%option yylineno


    /* definitions go here. period is an example. */
    /* NOTE: You do not need to use definitions */
letters          [a-z]
whitespace       [\t\n\r" " ]

%%
"//".*        { /* ignore single-line comment */ }
"/*"([^*]|\*+[^*/])*\*+"/"   { /* ignore block comments */ }
"("     { return '('; }
")"     { return ')';  }
"{"     { g_symbolTable.IncreaseScope(); return '{'; }
"}"     { g_symbolTable.DecreaseScope(); return '}'; }
"["     { return '['; }
"]"     { return ']'; }
";"     { return ';'; }
","     { return ','; }
"."     { return '.'; }
"+"     { return '+'; }
"-"     { return '-'; }
"*"     { return '*'; }
"/"     { return '/'; }
"%"     { return '%'; }
"="     { return '='; }
">"     { return '>'; }
"<"     { return '<'; }
 

"!="    { return NOT_EQUALS; }
"=="    { return EQUALS; }
"&&"    { return AND; }
"||"    { return OR; }
">="    { return GE; }
"<="    { return LE; }
 

"program"  { return PROGRAM; }
"if"       { return IF; }
"else"     { return ELSE; }
"endif"    { return ENDIF; }
"while"    { return WHILE; }
"print"    { return PRINT; }
"prints"   { return PRINTS; }
"char"     { return CHAR; }
"int"      { return INT; }
"long"     { return LONG; }
"float"    { return FLOAT; }
"double"   { return DOUBLE; }
"struct"   { return STRUCT; }
"array"    { return ARRAY; }
"return"   { return RETURN; }

 



[A-Za-z_][A-Za-z0-9_]* { return ProcessIdentifier(yytext); }

[1-9]*[0-9]+ {return INT_VAL;}

[0-9]+\.[0-9]+ { return FLOAT_VAL; }

\"([^\"\n]|\\\")*\"   { return STRING_LIT; }

[ \t\r\n]+     { /* ignore */ }

.              { return JUNK_TOKEN; }

%%
// This function allows us to do extra processing on each token
// It is used to generate lexer debug info
int Return(int val)
{
    printf("Scanned '%s': %d\n", yytext, val);
    return val;
}

//******************************************************
// Process_Identifier: create symbols in symbol table
static int ProcessIdentifier(char * inputIdentifier)
{
    string name(inputIdentifier);
    cSymbol *sym = nullptr;

    if (g_insert)
    {
        sym = g_symbolTable.FindLocal(name);
        if (sym == nullptr)
        {
            sym = new cSymbol(name);
            g_symbolTable.Insert(sym);
        }
    }
    else
    {
        sym = new cSymbol(name);
    }
    yylval.symbol = sym;

    return IDENTIFIER;
   
}