%{
//**************************************
// lang.y
//
// Parser definition file. bison uses this file to generate the parser.
//
// Author: Phil Howard 
//

#include <iostream>
#include "lex.h"
#include "astnodes.h"
#include "cSymbolTable.h"

// Flag set when a semantic error occurs in an AST node constructor
static bool g_semanticErrorHappened = false;

#define CHECK_ERROR() { if (g_semanticErrorHappened) \
    { g_semanticErrorHappened = false; } }
#define PROP_ERROR() { if (g_semanticErrorHappened) \
    { g_semanticErrorHappened = false; YYERROR; } }
    
%}

%locations

%code requires {
    class cVarDeclNode;
    class cDeclNode;
    class cDeclsNode;
    class cBlockNode;
    class cStmtsNode;
    class cExprNode;
    class cProgramNode;
    class cSymbol;
    class cVarExprNode;
    class cIfNode;
    class cReturnNode;
    class cWhileNode;
    class cArgsNode;
    class cParamsNode;
    class cFuncHeaderNode;
    class cFuncDeclNode;
    class cFuncCallNode;
} 

 /* union defines the type for lexical values */
%union{
    int             int_val;
    float           float_val;
    std::string*    str_val;
    cAstNode*       ast_node;
    cProgramNode*   program_node;
    cBlockNode*     block_node;
    cStmtsNode*     stmts_node;
    cStmtNode*      stmt_node;
    cExprNode*      expr_node;
    cIntExprNode*   int_node;
    cSymbol*        symbol;
    cDeclNode*      decl_node;
    cDeclsNode*     decls_node;
    cVarDeclNode*   var_decl_node;
    cVarExprNode*   var_expr_node;
    cIfNode*        if_node;
    cReturnNode*    return_node;
    }

%{
    int yyerror(const char *msg);

    cAstNode *yyast_root;
%}

// Macros for semantic error handling:
// CHECK_ERROR - note the error but continue parsing
// PROP_ERROR  - note the error AND trigger bison error recovery



%start  program

%token <symbol>    IDENTIFIER
%token <symbol>    TYPE_ID
%token <int_val>   CHAR_VAL
%token <int_val>   INT_VAL
%token <float_val> FLOAT_VAL
%token <int_val>   LE
%token <int_val>   GE
%token <int_val>   AND
%token <int_val>   OR
%token <int_val>   EQUALS
%token <int_val>   NOT_EQUALS
%token <str_val>   STRING_LIT

%token  PROGRAM
%token  PRINT PRINTS
%token  WHILE IF ELSE ENDIF
%token  STRUCT ARRAY
%token  RETURN
%token  JUNK_TOKEN

%type <program_node> program
%type <block_node> block
%type <ast_node> open
%type <ast_node> close
%type <decls_node> decls
%type <decl_node> decl
%type <var_decl_node> var_decl
%type <ast_node> struct_decl
%type <ast_node> array_decl
%type <ast_node> func_decl
%type <ast_node> func_header
%type <ast_node> func_prefix
%type <expr_node> func_call
%type <ast_node> paramsspec
%type <ast_node> paramspec
%type <stmts_node> stmts
%type <stmt_node> stmt
%type <ast_node> lval
%type <ast_node> params
%type <ast_node> param
%type <expr_node> expr
%type <expr_node> addit
%type <expr_node> term
%type <expr_node> fact
%type <expr_node> varref

%%

program: PROGRAM block
                                { $$ = new cProgramNode($2);
                                  yyast_root = $$;
                                  if (yynerrs == 0) 
                                      YYACCEPT;
                                  else
                                      YYABORT;
                                }
block:  open decls stmts close
                                { $$ = new cBlockNode($2,$3); }
    |   open stmts close
                                { $$ = new cBlockNode(nullptr, $2); }

open:   '{'
                                {   g_symbolTable.IncreaseScope();  }

close:  '}'
                                {   g_symbolTable.DecreaseScope();  }

decls:      decls decl
      {
            $1->Add($2);
            $$ = $1;
        }
        |   decl
                                { $$ = new cDeclsNode($1);  }
decl:       var_decl ';'
                                { $$ = $1; PROP_ERROR(); }
        |   array_decl ';'
                            { $$ = dynamic_cast<cDeclNode*>($1); }
        |   struct_decl ';'
                            { $$ = dynamic_cast<cDeclNode*>($1); }
        |   func_decl
                            { $$ = dynamic_cast<cDeclNode*>($1); }

var_decl:   TYPE_ID IDENTIFIER
        { $$ = new cVarDeclNode($1, $2); PROP_ERROR(); }

struct_decl:  STRUCT open decls close IDENTIFIER
                                { 
                                    // Check for duplicate before inserting
                                    if (g_symbolTable.FindLocal($5->GetName()) != nullptr)
                                    {
                                        SemanticParseError("Symbol " + $5->GetName() +
                                            " already defined in current scope");
                                    }
                                    else
                                    {
                                        cStructDeclNode* sd = new cStructDeclNode($3, $5);
                                        $5->SetDecl(sd);
                                        g_symbolTable.Insert($5);
                                    }
                                    $$ = new cStructDeclNode($3,$5);
                                    PROP_ERROR();
                                }
array_decl:   ARRAY TYPE_ID '[' INT_VAL ']' IDENTIFIER
                                {
                                    if (g_symbolTable.FindLocal($6->GetName()) != nullptr)
                                    {
                                        SemanticParseError("Symbol " + $6->GetName() +
                                            " already defined in current scope");
                                    }
                                    else
                                    {
                                        cArrayDeclNode* ad = new cArrayDeclNode($2, $6, $4);
                                        $6->SetDecl(ad);
                                        g_symbolTable.Insert($6);
                                        $$ = ad;
                                    }
                                    PROP_ERROR();
                                }

func_decl:  func_header ';'
                                { g_symbolTable.DecreaseScope(); $$ = new cFuncDeclNode(dynamic_cast<cFuncHeaderNode*>($1)); }
        |   func_header  '{' decls stmts '}'
                                { $$ = new cFuncDeclNode(dynamic_cast<cFuncHeaderNode*>($1), $3, $4); g_symbolTable.DecreaseScope(); }
        |   func_header  '{' stmts '}'
                                { $$ = new cFuncDeclNode(dynamic_cast<cFuncHeaderNode*>($1), $3); g_symbolTable.DecreaseScope(); }
func_header: func_prefix paramsspec ')'
                                { dynamic_cast<cFuncHeaderNode*>($1)->SetArgs(dynamic_cast<cArgsNode*>($2)); $$ = $1; }
        |    func_prefix ')'
                            { $$ = $1; }
func_prefix: TYPE_ID IDENTIFIER '('
                                { g_symbolTable.IncreaseScope(); $$ = new cFuncHeaderNode($1,$2); }
paramsspec:  paramsspec ',' paramspec
                                { $1->Add($3); $$ = $1; }
        |   paramspec
                            { $$ = new cArgsNode($1); }

paramspec:  var_decl
                                    { $$ = $1; }

stmts: stmts stmt
{
    $$ = $1;
    $$->Add($2);
}
        | stmt
{
    $$ = new cStmtsNode($1);
}

stmt:       IF '(' expr ')' stmts ENDIF ';'
                                { $$ = new cIfNode($3,$5); }
        |   IF '(' expr ')' stmts ELSE stmts ENDIF ';'
                                { $$ = new cIfNode($3,$5,$7); }
        |   WHILE '(' expr ')' stmt
                                { $$ = new cWhileNode($3,$5); }
        |   PRINT '(' expr ')' ';'
                                { $$ = new cPrintNode($3); }
        |   PRINTS '(' STRING_LIT ')' ';'
                                { $$ = new cPrintsNode(*$3); delete $3; }
        |   lval '=' expr ';'
                            { $$ = new cAssignNode(dynamic_cast<cVarExprNode*>($1), $3); }
        |   func_call ';'
                            { $$ = dynamic_cast<cStmtNode*>($1); }
        |   block
                            { $$ = $1; }
        |   RETURN expr ';'
            { 
                $$ = new cReturnNode($2);
            }
        |   error ';'
                            {}

func_call:  IDENTIFIER '(' params ')'
                                    { $$ = new cFuncCallNode($1, dynamic_cast<cParamsNode*>($3)); }
        |   IDENTIFIER '(' ')'
                            { $$ = new cFuncCallNode($1, nullptr); }

varref: IDENTIFIER
      {
            $$ = new cVarExprNode($1); CHECK_ERROR();
      }
    |   varref '.' IDENTIFIER
      {
            $1->Add($3);
            $$ = $1;
      }
    |   varref '[' expr ']'
      {
            $1->Add($3);
            $$ = $1;
      }
    ;

lval:     varref
                                { $$ = $1; }

params:   params ',' param
                                { $1->Add($3); $$ = $1; }
        |   param
                            { $$ = new cParamsNode($1); }

param:      expr
                                { $$ = $1; }

expr:       expr EQUALS addit
{ $$ = new cBinaryExprNode($1, new cOpNode(EQUALS), $3); }
    |   expr NOT_EQUALS addit
{ $$ = new cBinaryExprNode($1, new cOpNode(NOT_EQUALS), $3); }
    |   expr '<' addit
{ $$ = new cBinaryExprNode($1, new cOpNode('<'), $3); }
    |   expr '>' addit
{ $$ = new cBinaryExprNode($1, new cOpNode('>'), $3); }
    |   expr LE addit
{ $$ = new cBinaryExprNode($1, new cOpNode(LE), $3); }
    |   expr GE addit
{ $$ = new cBinaryExprNode($1, new cOpNode(GE), $3); }
    |   expr AND addit
{ $$ = new cBinaryExprNode($1, new cOpNode(AND), $3); }
    |   expr OR addit
{ $$ = new cBinaryExprNode($1, new cOpNode(OR), $3); }
    |   addit
{ $$ = $1; }

addit: addit '+' term
{ $$ = new cBinaryExprNode($1, new cOpNode('+'), $3); }
        | addit '-' term
{ $$ = new cBinaryExprNode($1, new cOpNode('-'), $3); }
        | term
{ $$ = $1; };

term: term '*' fact
{ $$ = new cBinaryExprNode($1, new cOpNode('*'), $3); }
| term '/' fact
{ $$ = new cBinaryExprNode($1, new cOpNode('/'), $3); }
| term '%' fact
{ $$ = new cBinaryExprNode($1, new cOpNode('%'), $3); }
| fact
{ $$ = $1; };

fact:       '(' expr ')'
                                { $$ = $2; }
        |   INT_VAL
                            { $$ = new cIntExprNode($1); }
        |   FLOAT_VAL
                            { $$ = new cFloatExprNode($1); }
        |   varref
                            { $$ = $1; }
        |   func_call
                            { $$ = $1; }

%%

// Function to format error messages
int yyerror(const char *msg)
{
    std::cerr << "ERROR: " << msg << " at symbol "
        << yytext << " on line " << yylineno << "\n";

    return 0;
}

// Function that gets called when a semantic error happens during parsing
void SemanticParseError(std::string error)
{
    std::cout << "ERROR: " << error << " near line "
              << yylineno << "\n";
    g_semanticErrorHappened = true;
    yynerrs++;
}
