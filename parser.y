/*INF UFRGS 2018/2// COMPILADORES SCHNORR // GRUPO KAPPA // GUILHERME CATTANI DE CASTRO 00243589 && CAIO F RODRIGUES 00261578*/
%{
#include <stdio.h>
int yylex(void);
void yyerror (char const *s);

extern int yylineno;
%}

%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_CHAR
%token TK_PR_STRING
%token TK_PR_IF
%token TK_PR_THEN
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_DO
%token TK_PR_INPUT
%token TK_PR_OUTPUT
%token TK_PR_RETURN
%token TK_PR_CONST
%token TK_PR_STATIC
%token TK_PR_FOREACH
%token TK_PR_FOR
%token TK_PR_SWITCH
%token TK_PR_CASE
%token TK_PR_BREAK
%token TK_PR_CONTINUE
%token TK_PR_CLASS
%token TK_PR_PRIVATE
%token TK_PR_PUBLIC
%token TK_PR_PROTECTED
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_OC_SL
%token TK_OC_SR
%token TK_OC_FORWARD_PIPE
%token TK_OC_BASH_PIPE
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_LIT_CHAR
%token TK_LIT_STRING
%token TK_IDENTIFICADOR
%token TOKEN_ERRO

%%

programa:
    declaration;


/*obvious stuff */

id:
    TK_IDENTIFICADOR;

type:
    TK_PR_INT | TK_PR_FLOAT | TK_PR_CHAR | TK_PR_BOOL | TK_PR_STRING | id;

//Available types for class declaration
classAvailableType:
    TK_PR_INT | TK_PR_FLOAT | TK_PR_CHAR | TK_PR_BOOL | TK_PR_STRING

//Modifiers are usually optional
accessModifiers:
    TK_PR_PRIVATE | TK_PR_PUBLIC | TK_PR_PROTECTED | %empty;

staticModifier:
    TK_PR_STATIC | %empty;
    
/*obvious stuff end */


varDeclaration:
    type id;


declaration:
    classDeclaration | globalVarDeclaration;


/* A class is a declaration of a new type in the format:
    class id1 {
        protected int id2:
        protected float id3
    }; 
*/
classDeclaration:
    accessModifiers TK_PR_CLASS id '{' fields '}' ';';   

// fields are varDeclarations inside classes that can't contain user-classes nor initialization of values
fields:
    accessModifiers classAvailableType id | accessModifiers classAvailableType id ':' fields;


//Declaration of global variables
globalVarDeclaration:
    staticModifier varDeclaration ';' ;


%%

void yyerror (char const *s){
    printf("%d:ERROR: %s\n",yylineno, s);
}