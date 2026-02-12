%{
//**************************************
// lang.l
//
// Scanner definition file
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

// Illustration showing how to use {} to control scope
    #include "cSymbol.h"
#include "cSymbolTable.h"
#include "lex.h"
#include "langparse.h"

// Macros for optional debug output
// Uncomment to enable lexer debug printing
//#define DEBUG_OUTPUT
#ifdef DEBUG_OUTPUT
#define DO_RETURN(a) { return Return(a); }
#else 
#define DO_RETURN(a) { return (a); }
#endif 
int Return(int val);
int yyinput(void);
//static int ProcessIdentifier(const char *text);
%}

%option noyywrap
%option noinput
%option nounput
%option yylineno

%x STRING
%{ 
static std::string _string_buf;
%}

/* Definitions */
whitespace [\t\n\r ]
letters     [a-zA-Z_]
%%

    // token definitions. I gave "program" as an example
[ \t\r\n]+            { /* skip whitespace */ }

"//".*                { /* skip comment */ }

"*"                   { return '*'; }
"/"                   { return '/'; }
"%"                   { return '%'; }
"+"                   { return '+'; }
"-"                   { return '-'; }

"=="                  { DO_RETURN(EQUALS); }
"!="                  { DO_RETURN(NOT_EQUALS); }
"&&"                  { DO_RETURN(AND); }
"||"                  { DO_RETURN(OR); }
"<="                  { DO_RETURN(LE); }
">="                  { DO_RETURN(GE); }
"\""    { BEGIN(STRING); _string_buf.clear(); }

<STRING>[^\"\n]+    { _string_buf.append(yytext, yyleng); }

<STRING>\"    { yylval.str_val = new std::string(_string_buf); BEGIN(INITIAL); DO_RETURN(STRING_LIT); }

program                     DO_RETURN(PROGRAM);
print                       DO_RETURN(PRINT);
"prints"                    return PRINTS;
"while"                     return WHILE;
"if"                        return IF;
"else"                      return ELSE;
"endif"                     return ENDIF;
"struct"                    return STRUCT;
"array"                     return ARRAY;
[0-9]+\.[0-9]+              {yylval.float_val = atof(yytext); DO_RETURN(FLOAT_VAL);}
[0-9]+                      {yylval.int_val = atoi(yytext); DO_RETURN(INT_VAL);}
"["                    DO_RETURN('[');
"]"                    DO_RETURN(']');
"."                    DO_RETURN('.');
"{"                    DO_RETURN('{');
"}"                    DO_RETURN('}');
";"                    DO_RETURN(';');
"("                    DO_RETURN('(');
")"                    DO_RETURN(')');
","                    DO_RETURN(',');
"="                    DO_RETURN('=');
"<"                    DO_RETURN('<');
">"                    DO_RETURN('>');
"return"                    DO_RETURN(RETURN);
"char"       { 
    cSymbol* sym = g_symbolTable.Find("char");
    if (sym == nullptr) {
        sym = new cSymbol("char");
        sym->SetIsType(true);
        g_symbolTable.Insert(sym);
    }
    yylval.symbol = sym; 
    return TYPE_ID; 
}
"int"       {
    cSymbol* sym = g_symbolTable.Find("int");
    if (sym == nullptr) {
        sym = new cSymbol("int");
        sym->SetIsType(true);
        g_symbolTable.Insert(sym);
    }
    yylval.symbol = sym;
    return TYPE_ID;
}

"float"       {
    cSymbol* sym = g_symbolTable.Find("float");
    if (sym == nullptr) {
        sym = new cSymbol("float");
        sym->SetIsType(true);
        g_symbolTable.Insert(sym);
    }
    yylval.symbol = sym;
    return TYPE_ID;
}

"long"       {
    cSymbol* sym = g_symbolTable.Find("long");
    if (sym == nullptr) {
        sym = new cSymbol("long");
        sym->SetIsType(true);
        g_symbolTable.Insert(sym);
    }
    yylval.symbol = sym;
    return TYPE_ID;
}

"double"       {
    cSymbol* sym = g_symbolTable.Find("double");
    if (sym == nullptr) {
        sym = new cSymbol("double");
        sym->SetIsType(true);
        g_symbolTable.Insert(sym);
    }
    yylval.symbol = sym;
    return TYPE_ID;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    cSymbol* sym = g_symbolTable.Find(yytext);
    if (sym == nullptr) {
        sym=new cSymbol(yytext);
        g_symbolTable.Insert(sym);
    }
    yylval.symbol = sym;
    
    if (sym->GetIsType()) {
        return TYPE_ID;
    } else {
        return IDENTIFIER;
    }
}
%%

//******************************************************
//******************************************************
// Return: optional debug output for tokens
int Return(int val)
{
	printf("Scanned '%s': %d\n", yytext, val);
	return val;
}
//******************************************************
// ProcessID: create symbols in symbol table
/*
static int ProcessIdentifier(const char *text)
{
    cSymbol* symbol = g_symbolTable.FindLocal(text);
    if(!symbol)
    {
        symbol=new cSymbol(text);
        g_symbolTable.Insert(symbol);
    }
    yylval.symbol=symbol;
    return IDENTIFIER;
}
*/
