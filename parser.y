/*INF UFRGS 2018/2// COMPILADORES SCHNORR // GRUPO KAPPA // GUILHERME CATTANI DE CASTRO 00243589 && CAIO F RODRIGUES 00261578*/
%code requires { 
    #define FALSE             0
    #define TRUE              1

    #define RESERVED_WORD     1
    #define SPECIAL_CHARACTER 2
    #define COMPOUND_OPERATOR 3
    #define IDENTIFIER        4

    #define LITERAL_INT       5
    #define LITERAL_BOOL      6
    #define LITERAL_FLOAT     7
    #define LITERAL_CHAR      8
    #define LITERAL_STRING    9

    #define AST_PROGRAMA      "Estado Programa"
    #define AST_LITERAL       "Estado literal"
    #define AST_PIPE          "Estado pipe"
    #define AST_USERVARORLIT  "Estado userVariableOrLiteral"
    #define AST_TYPE          "Estado type"
    #define AST_PRIMITIVETYPE "Estado primitiveType"
    #define AST_SHIFT         "Estado shift"
    #define AST_ACCESSMOD     "Estado accessModifiers"
    #define AST_CONSTMOD      "Estado constModifier"
    #define AST_VECTORMOD     "Estado vectorModifier"
    #define AST_VECTORLIST    "Estado vectorList"
    #define AST_CODE          "Estado code"
    #define AST_DECLARATION   "Estado declaration"
    #define AST_CLASSDEC      "Estado classDeclaration"
    #define AST_FIELDS        "Estado fields"
    #define AST_GLOBALVARDEC  "Estado globalVarDeclaration"
    #define AST_FUNCDEC       "Estado functionDeclaration"
    #define AST_FUNCHEAD      "Estado functionHead"
    #define AST_COMMANDSBLOCK "Estado commandsBlock"
    #define AST_COMMANDSLIST  "Estado commandsList"
    #define AST_FUNCARGLIST   "Estado functionArgumentsList"
    #define AST_FUNCARGELEM   "Estado functionArgumentElements"
    #define AST_COMMAND       "Estado command"
    #define AST_COMMANDSIMPLE "Estado commandSimple"
    #define AST_LOCALVARDEC   "Estado localVarDeclaration"
    #define AST_LOCALVARCDEC  "Estado localVarCompleteDeclaration"
    #define AST_ATTRIBUTION   "Estado attribution"
    #define AST_PRIMATTR      "Estado primitiveAttribution"
    #define AST_USERTYPEATTR  "Estado userTypeAttribution"
    #define AST_SIMPLEEXP     "Estado simpleExpression"
    #define AST_LOWPTFREXP    "Estado lowPrecedenceTwoFoldRecursiveExpression"
    #define AST_MEDPTFREXP    "Estado mediumPrecedenceTwoFoldRecursiveExpression"
    #define AST_HIGHPTFREXP   "Estado highPrecedenceTwoFoldRecursiveExpression"
    #define AST_ONEFREXP      "Estado oneFoldRecursiveExpression"
    #define AST_OPERATOR      "Estado operator"
    #define AST_UNARYSIMBOL   "Estado unarySimbol"
    #define AST_ARITHMETICOP  "Estado arithmeticOperator"
    #define AST_PRECARITHOP   "Estado precedentArithmeticOperator"
    #define AST_COMPARISONOP  "Estado comparisonOperator"
    #define AST_EXPLIST       "Estado expressionList"
    #define AST_FUNCTIONCALL  "Estado functionCall"
    #define AST_FUNCCALLARGS  "Estado functionCallArguments"
    #define AST_FUNCCALLARGSL "Estado functionCallArgumentsList"
    #define AST_FUNCCALLARG   "Estado functionCallArgument"
    #define AST_SHIFTCOMMAND  "Estado shiftCommand"
    #define AST_FLUXCONTCMD   "Estado fluxControlCommand"
    #define AST_CONDFLUXCONT  "Estado conditionalFluxControl"
    #define AST_ITERFLUXCONT  "Estado iterativeFluxControl"
    #define AST_SELECFLUXCONT "Estado selectionFluxControl"
    #define AST_COMMANDSIMPLELIST "Estado commandSimpleList"
    #define AST_PIPECOMMANDS  "Estado pipeCommands"



    #include <stdio.h>
    #include <string.h>
    #include "ASTree.h"

    int yylex(void);
    void yyerror (char const *s);

    extern int yylineno;
    extern char* yytext;
    extern void* arvore;



    
}


%union{
    struct node* nodo;
};

%token<nodo> TK_PR_INT
%token<nodo> TK_PR_FLOAT
%token<nodo> TK_PR_BOOL
%token<nodo> TK_PR_CHAR
%token<nodo> TK_PR_STRING
%token<nodo> TK_PR_IF
%token<nodo> TK_PR_THEN
%token<nodo> TK_PR_ELSE
%token<nodo> TK_PR_WHILE
%token<nodo> TK_PR_DO
%token<nodo> TK_PR_INPUT
%token<nodo> TK_PR_OUTPUT
%token<nodo> TK_PR_RETURN
%token<nodo> TK_PR_CONST
%token<nodo> TK_PR_STATIC
%token<nodo> TK_PR_FOREACH
%token<nodo> TK_PR_FOR
%token<nodo> TK_PR_SWITCH
%token<nodo> TK_PR_CASE
%token<nodo> TK_PR_BREAK
%token<nodo> TK_PR_CONTINUE
%token<nodo> TK_PR_CLASS
%token<nodo> TK_PR_PRIVATE
%token<nodo> TK_PR_PUBLIC
%token<nodo> TK_PR_PROTECTED
%token<nodo> TK_OC_LE
%token<nodo> TK_OC_GE
%token<nodo> TK_OC_EQ
%token<nodo> TK_OC_NE
%token<nodo> TK_OC_AND
%token<nodo> TK_OC_OR
%token<nodo> TK_OC_SL
%token<nodo> TK_OC_SR
%token<nodo> TK_OC_FORWARD_PIPE
%token<nodo> TK_OC_BASH_PIPE
%token<nodo> TK_LIT_INT
%token<nodo> TK_LIT_FLOAT
%token<nodo> TK_LIT_FALSE
%token<nodo> TK_LIT_TRUE
%token<nodo> TK_LIT_CHAR
%token<nodo> TK_LIT_STRING                
%token<nodo> TK_IDENTIFICADOR                    
%token<nodo> ';' ',' ':' ')' '(' ']' '[' '}' '{' '+' '-' '|' '?' '*' '/' '>' '<' '=' '!' '&' '%' '#' '^' '.' '$';

%token TOKEN_ERRO
 
%type<nodo> programa
%type<nodo> code
%type<nodo> declaration
%type<nodo> globalVarDeclaration
%type<nodo> type

%type<nodo> test

%%

programa:
    test    {arvore = createChildren(createNode(AST_PROGRAMA), $1);};

//code {printf("endofcode\n");struct node* root = createChildren(createRootNode(), $1); showTree(root);}
//| %empty;

test:
    TK_IDENTIFICADOR TK_IDENTIFICADOR ';'
    
     {
       $$ = createNode(AST_ACCESSMOD); 
       createChildren($$, $1);
       createChildren($$, $2); 
       createChildren($$, $3); 
     };

/*obvious stuff */

literal:
      TK_LIT_CHAR 
    | TK_LIT_FALSE 
    | TK_LIT_FLOAT 
    | TK_LIT_INT 
    | TK_LIT_STRING 
    | TK_LIT_TRUE;

pipe:
      TK_OC_BASH_PIPE
    | TK_OC_FORWARD_PIPE;

userVariableOrLiteral:
      TK_IDENTIFICADOR //userVariable
    | literal;

type:
      TK_PR_INT 
    | TK_PR_FLOAT 
    | TK_PR_CHAR 
    | TK_PR_BOOL 
    | TK_PR_STRING 
    | TK_IDENTIFICADOR; //userType

//Available types for class declaration
primitiveType:
      TK_PR_INT 
    | TK_PR_FLOAT 
    | TK_PR_CHAR 
    | TK_PR_BOOL 
    | TK_PR_STRING;

shift:
      TK_OC_SR 
    | TK_OC_SL;
//Modifiers are usually optional
accessModifiers:
      TK_PR_PRIVATE 
    | TK_PR_PUBLIC 
    | TK_PR_PROTECTED 
    | %empty;

constModifier:
      TK_PR_CONST 
    | %empty;

vectorModifier:
     '[' expression ']' vectorModifier
    | %empty;

vectorList:
      '[' expression ']'
    | vectorList '[' expression ']';
/*obvious stuff end */

code:
      declaration {$$=$1;}
    | declaration code;

declaration:
      classDeclaration 
    | globalVarDeclaration {$$ = $1;}
    | functionDeclaration; 



/* A class is a declaration of a new type in the format:
    class id1 {
        protected int id2:
        protected float id3
    }; 
*/
classDeclaration:
    accessModifiers TK_PR_CLASS TK_IDENTIFICADOR '[' fields ']' ';';   //className

// fields are type and id inside classes that can't contain user-classes nor initialization of values
fields:
      accessModifiers primitiveType TK_IDENTIFICADOR //classField
    | accessModifiers primitiveType TK_IDENTIFICADOR ':' fields; //classField

//Declaration of global variables
globalVarDeclaration:
    TK_IDENTIFICADOR type  ';'
    | TK_IDENTIFICADOR TK_PR_STATIC type  ';' //userVariable userType
    | TK_IDENTIFICADOR '['TK_LIT_INT']' type  ';' //userVariable userType
    | TK_IDENTIFICADOR '['TK_LIT_INT']' TK_PR_STATIC type  ';' ; //userVariable userType

//Function declaration
functionDeclaration:
    functionHead commandsBlock;

functionHead:
      primitiveType TK_IDENTIFICADOR '(' functionArgumentsList ')' //userType functionName
    | TK_IDENTIFICADOR TK_IDENTIFICADOR '(' functionArgumentsList ')' //userType functionName
    | TK_PR_STATIC primitiveType TK_IDENTIFICADOR  '(' functionArgumentsList ')' // userType functionName
    | TK_PR_STATIC TK_IDENTIFICADOR TK_IDENTIFICADOR  '(' functionArgumentsList ')' // userType functionName

commandsBlock:
    '{' commandsList '}';

commandsList:
      command commandsList
    | %empty;

functionArgumentsList:
      functionArgumentElements 
    | %empty;

functionArgumentElements:
      constModifier type TK_IDENTIFICADOR //userVariable
    | constModifier type TK_IDENTIFICADOR ',' functionArgumentElements; //userVariable

//All Commands, it can be anypart of a simple command or it can be a case/output
command:
      commandSimple ';' //All simple commands finish with a ';'
    | TK_PR_OUTPUT expressionList ';' //Output has parenthesis and thus is not a simple command
    | TK_PR_CASE TK_LIT_INT ':'; //case from switch cannot end in ';'

//Commands without commas or case: Used in the "for" command
commandSimple:
      localVarCompleteDeclaration
    | attribution
    | TK_PR_INPUT expression 
    | functionCall;
    | shiftCommand;
    | TK_PR_RETURN expression
    | TK_PR_CONTINUE
    | TK_PR_BREAK
    | fluxControlCommand
    | pipeCommands
    | commandsBlock;      

//A variable declaration can be initialized ONLY if it has a primitive type
localVarDeclaration:
      primitiveType TK_IDENTIFICADOR localVarInit //If it starts with ID and is initialized
    | TK_IDENTIFICADOR TK_IDENTIFICADOR;  //userType userVariable

localVarCompleteDeclaration:
      TK_PR_STATIC TK_PR_CONST localVarDeclaration 
    | TK_PR_STATIC localVarDeclaration 
    | TK_PR_CONST localVarDeclaration 
    | localVarDeclaration;
    
//Initialization of variable
localVarInit:
      TK_OC_LE userVariableOrLiteral 
    | %empty;

attribution:
      primitiveAttribution
    | userTypeAttribution;

primitiveAttribution:
      TK_IDENTIFICADOR vectorModifier '=' expression ;  //userVariable
 //userVariable

userTypeAttribution:
      TK_IDENTIFICADOR vectorModifier '$' TK_IDENTIFICADOR '=' expression ; //userVariable

simpleExpression:
      TK_IDENTIFICADOR
    | functionCall
    | pipeCommands
    | literal
    | TK_IDENTIFICADOR  '$' TK_IDENTIFICADOR;

expression:
    lowPrecedenceTwoFoldRecursiveExpression
    | lowPrecedenceTwoFoldRecursiveExpression '?' expression ':' expression;

lowPrecedenceTwoFoldRecursiveExpression:
    mediumPrecedenceTwoFoldRecursiveExpression
    | lowPrecedenceTwoFoldRecursiveExpression operator mediumPrecedenceTwoFoldRecursiveExpression;

mediumPrecedenceTwoFoldRecursiveExpression:
    highPrecedenceTwoFoldRecursiveExpression
    | mediumPrecedenceTwoFoldRecursiveExpression precedentArithmeticOperator highPrecedenceTwoFoldRecursiveExpression;

highPrecedenceTwoFoldRecursiveExpression:
    oneFoldRecursiveExpression
    | highPrecedenceTwoFoldRecursiveExpression '^' oneFoldRecursiveExpression;

oneFoldRecursiveExpression:
     simpleExpression
    | unaryOperator simpleExpression;

    | '(' expression ')'
    | unaryOperator '(' expression ')'

    | TK_IDENTIFICADOR vectorList;
    | unaryOperator TK_IDENTIFICADOR vectorList;

    | TK_IDENTIFICADOR vectorList '$' TK_IDENTIFICADOR;
    | unaryOperator TK_IDENTIFICADOR vectorList '$' TK_IDENTIFICADOR;

operator:
      arithmeticOperator
    | comparisonOperator;

unaryOperator:
       unarySimbol
    |  unaryOperator unarySimbol;

unarySimbol:
      '+' 
    | '-'
    | '!'
    | '&'
    | '*'
    | '?'
    | '#';

arithmeticOperator:
      '+' 
    | '-'
    | '|'
    | '&';

precedentArithmeticOperator:
      '*'
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

//Function call has a very straight forward approach
functionCall:
    TK_IDENTIFICADOR '(' functionCallArguments ')';

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
      TK_IDENTIFICADOR vectorModifier shift expression
    | TK_IDENTIFICADOR vectorModifier '$' TK_IDENTIFICADOR shift expression;

//Flux Control can be of 3 kinds: Conditional, iterative or selection
fluxControlCommand:
      conditionalFluxControl;
    | iterativeFluxControl
    | selectionFluxControl;

//Conditional flux is if (exp) then {...} else {...}, with the else being optional
conditionalFluxControl:
      TK_PR_IF '(' expression ')' TK_PR_THEN commandsBlock
    | TK_PR_IF '(' expression ')' TK_PR_THEN commandsBlock TK_PR_ELSE commandsBlock;

//There are 4 variations of iterative flux control
iterativeFluxControl:
      TK_PR_FOREACH '(' TK_IDENTIFICADOR ':' expressionList ')' commandsBlock;
    | TK_PR_FOR '(' commandSimpleList ':' expression ':' commandSimpleList ')' commandsBlock //The command list is of simple commands
    | TK_PR_WHILE '(' expression ')' TK_PR_DO commandsBlock
    | TK_PR_DO commandsBlock TK_PR_WHILE '(' expression ')';      

//The only selection flux control is switch
//It is important to note that bison won't check if there are cases on the commandsBlock
selectionFluxControl:
    TK_PR_SWITCH '(' expression ')' commandsBlock;

//List of commands without commas in them, used in for
commandSimpleList:
      commandSimple 
    | commandSimpleList ',' commandSimple ;

//Pipe Commands can be in the format of "f() %>% f()" or "f() %>% f() %>% ... f()"
pipeCommands:
      functionCall pipe functionCall
    | pipeCommands pipe functionCall;

%%


void yyerror (char const *s){
    if(yytext == NULL || yytext[0] == '\0')
        printf("Line %d: %s in last token of line\n",yylineno, s);
    else
        printf("Line %d: %s near \"%s\"\n",yylineno, s, yytext);
}


void descompila (void *arvore){
    showTreeRecursion(arvore, 0);
}
void libera (void *arvore){
    liberaTree(arvore);
}