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
      code | %empty;

/*obvious stuff */

id:
    TK_IDENTIFICADOR;

userType:
    TK_IDENTIFICADOR;

classField:
    TK_IDENTIFICADOR;

literal:
      TK_LIT_CHAR 
    | TK_LIT_FALSE 
    | TK_LIT_FLOAT 
    | TK_LIT_INT 
    | TK_LIT_STRING 
    | TK_LIT_TRUE;

comparableLiteral:
    TK_LIT_FALSE | TK_LIT_FLOAT | TK_LIT_INT | TK_LIT_TRUE;

number:
    TK_LIT_FLOAT | TK_LIT_INT;

idOrLiteral:
    id | literal;

type:
      TK_PR_INT 
    | TK_PR_FLOAT 
    | TK_PR_CHAR 
    | TK_PR_BOOL 
    | TK_PR_STRING 
    | id;

//Available types for class declaration
primitiveType:
      TK_PR_INT 
    | TK_PR_FLOAT 
    | TK_PR_CHAR 
    | TK_PR_BOOL 
    | TK_PR_STRING

shift:
    TK_OC_SR | TK_OC_SL;
//Modifiers are usually optional
accessModifiers:
      TK_PR_PRIVATE 
    | TK_PR_PUBLIC 
    | TK_PR_PROTECTED 
    | %empty;

staticModifier:
    TK_PR_STATIC 
    | %empty;

constModifier:
    TK_PR_CONST 
    | %empty;

vectorModifier:
     '[' expression ']' 
    | %empty;
/*obvious stuff end */

code:
    declaration 
    | declaration code;

declaration:
      classDeclaration 
    | globalVarDeclaration 
    | functionDeclaration;


/* A class is a declaration of a new type in the format:
    class id1 {
        protected int id2:
        protected float id3
    }; 
*/
classDeclaration:
    accessModifiers TK_PR_CLASS id '{' fields '}' ';';   

// fields are type and id inside classes that can't contain user-classes nor initialization of values
fields:
    accessModifiers primitiveType id 
    | accessModifiers primitiveType id ':' fields;


//Declaration of global variables
globalVarDeclaration:
    staticModifier type id vectorModifier ';' ;

//Function declaration
functionDeclaration:
    functionHead functionBody;

functionHead:
    staticModifier type id '(' functionArgumentsList ')'

functionBody:
    '{' commandsBlock '}';

commandsBlock:
    command ';' commandsBlock | %empty | ';' commandsBlock | functionBody;

functionArgumentsList:
    functionArgumentElements | %empty;

functionArgumentElements:
    constModifier type id 
    | constModifier type id ',' functionArgumentElements;

//Simple Command
command:
      localVarCompleteDeclaration
    | attribution
    | inputOutputCommand
    | functionCall;
    | shiftCommand;
    | fluxControlCommand;

//A variable declaration can be initialized ONLY if it has a primitive type
localVarDeclaration:
      primitiveType id localVarInit //If it starts with ID and is initialized
    | userType id;  


localVarCompleteDeclaration:
      TK_PR_STATIC TK_PR_CONST localVarDeclaration 
    | TK_PR_CONST localVarDeclaration 
    | localVarDeclaration;


    
//Initialization of variable
localVarInit:
    TK_OC_LE idOrLiteral | %empty;

attribution:
    primitiveAttribution | userTypeAttribution;

primitiveAttribution:
    id vectorModifier '=' expression ;

userTypeAttribution:
    id vectorModifier '$' classField '=' expression ;

expression:
      comparableLiteral 
    | '-' expression
    | compositeExpression;

compositeExpression:
      comparableLiteral operator comparableLiteral operator expression;
    | comparableLiteral operator comparableLiteral;

operator:
      arithmeticOperator
    | comparisonOperator;

arithmeticOperator:
      '*' 
    | '+'
    | '-'
    | '/'
    | '%';

comparisonOperator:
      TK_OC_LE
    | TK_OC_GE
    | TK_OC_EQ
    | TK_OC_NE
    | TK_OC_AND
    | TK_OC_OR
    | '<'
    | '>';

expressionList:
      expression 
    | expression ',' expressionList

inputOutputCommand:
      TK_PR_INPUT expression
    | TK_PR_OUTPUT expressionList;

//Function call has a very straight forward approach
functionCall:
    id '(' functionCallArguments ')';

//Arguments can be empty or can be a list of expressions/dots
functionCallArguments:
      functionCallArgumentsList
    | %empty;

//List of Expression/Dots
functionCallArgumentsList:
    functionCallArgument
    | functionCallArgument ',' functionCallArgumentsList
    
//Argument can be expression or dot
functionCallArgument:
      '.'
    | expression;

//Shift command is straightforward too
shiftCommand:
      id vectorModifier shift number
    | id vectorModifier '.' id shift number;

//WIP
fluxControlCommand:
      conditionalFluxControl;
 //   | selectionFluxControl;

//WIP², works but always needs curly brackets
conditionalFluxControl:
      TK_PR_IF '(' expression ')' TK_PR_THEN functionBody
    | TK_PR_IF '(' expression ')' TK_PR_THEN functionBody TK_PR_ELSE functionBody;

/*
selectionFluxControl:
      TK_PR_FOREACH '(' id ':' expressionList ')' functionBody;
    | TK_PR_FOR '(' commandList ':' expression ':' commandList ')' functionBody
    | TK_PR_WHILE '(' expression ')' TK_PR_DO functionBody
    | TK_PR_DO functionBody TK_PR_WHILE '(' expression ')';      

commandList:
    command | command ',' commandList;
*/
%%

void yyerror (char const *s){
    printf("%d:ERROR: %s\n",yylineno, s);
}