//**************************************
// main.cpp
//
// Main routine for lang compiler.
// This version only runs the lexer
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 23, 2015
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include "cSymbol.h"
#include "cSymbolTable.h"
#include "lex.h"
#include "astnodes.h"
#include "langparse.h"

// Uncomment the following line after integrating your symbol table with
// your scanner.
#define TEST2

//****************************************
// argv[1] contains the file to process
// argv[2] if given, contains the name of the output file

int g_insert = false;
cSymbolTable g_symbolTable;
long long cSymbol::nextId;

int main(int argc, char **argv)
{
    //std::cout << "Parker Fagen" << std::endl;

    const char *outfile_name;
    int result = 0;
    int token;
    int do_test2 = 0;

    if (argc > 1)
    {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL)
        {
            std::cerr << "Unable to open file " << argv[1] << "\n";
            exit(-1);
        }
    }

    if (argc > 2)
    {
        outfile_name = argv[2];
        FILE *output = fopen(outfile_name, "w");
        if (output == NULL)
        {
            std::cerr << "Unable to open output file " << outfile_name << "\n";
            exit(-1);
        }
        int output_fd = fileno(output);
        if (dup2(output_fd, 1) != 1)
        {
            std::cerr << "Unable to send output to " << outfile_name << "\n";
            exit(-2);
        }
    }

    if (argc > 3) do_test2 = 1;
    // Add standard types to the outermost scope
    // In main(), before yyparse():
// Insert and mark standard types as types
    cSymbol* char_sym = new cSymbol("char");
char_sym->SetIsType(true);
g_symbolTable.Insert(char_sym);

cSymbol* int_sym = new cSymbol("int");
int_sym->SetIsType(true);
g_symbolTable.Insert(int_sym);

cSymbol* float_sym = new cSymbol("float");
float_sym->SetIsType(true);
g_symbolTable.Insert(float_sym);

cSymbol* long_sym = new cSymbol("long");
long_sym->SetIsType(true);
g_symbolTable.Insert(long_sym);

cSymbol* double_sym = new cSymbol("double");
double_sym->SetIsType(true);
g_symbolTable.Insert(double_sym);
result = yyparse();
    if (yyast_root != nullptr)
    {
        if (result == 0)
        {
            std::cout << yyast_root->ToString();
        } else {
            std::cout << " Errors in compile\n";
        }
    }

    if (result == 0 && yylex() != 0)
    {
        std::cout << "Junk at end of program\n";
    }

    token = yylex();
    while (token != 0)
    {
#ifdef TEST2
        if (do_test2 && token == IDENTIFIER)
            printf("%d:%s:%lld\n", token, yytext, yylval.symbol->GetId());
        else
            printf("%d:%s\n", token, yytext);
#else
        if (do_test2)
        {
            fprintf(stderr, "Not compiled with TEST2 defined\n");
            return 0;
        }
        else
            printf("%d:%s\n", token, yytext);
#endif

        token = yylex();
    }

    return result;
}

