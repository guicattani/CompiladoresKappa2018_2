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
    #define AST_LOCALVARINIT  "Estado localVarInit"
    #define AST_ATTRIBUTION   "Estado attribution"
    #define AST_PRIMATTR      "Estado primitiveAttribution"
    #define AST_USERTYPEATTR  "Estado userTypeAttribution"
    #define AST_EXPRESSION    "Estado expression"
    #define AST_SIMPLEEXP     "Estado simpleExpression"
    #define AST_LOWPTFREXP    "Estado lowPrecedenceTwoFoldRecursiveExpression"
    #define AST_MEDPTFREXP    "Estado mediumPrecedenceTwoFoldRecursiveExpression"
    #define AST_HIGHPTFREXP   "Estado highPrecedenceTwoFoldRecursiveExpression"
    #define AST_ONEFREXP      "Estado oneFoldRecursiveExpression"
    #define AST_OPERATOR      "Estado operator"
    #define AST_UNARYOPERATOR "Estado unaryOperator"
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

    extern struct nodeList* nodeList;

    
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
%type<nodo> literal
%type<nodo> pipe
%type<nodo> userVariableOrLiteral
%type<nodo> type
%type<nodo> primitiveType
%type<nodo> shift
%type<nodo> accessModifiers
%type<nodo> constModifier
%type<nodo> vectorModifier
%type<nodo> vectorList
%type<nodo> code
%type<nodo> declaration
%type<nodo> classDeclaration
%type<nodo> fields
%type<nodo> globalVarDeclaration
%type<nodo> functionDeclaration
%type<nodo> functionHead
%type<nodo> commandsBlock
%type<nodo> commandsList
%type<nodo> functionArgumentsList
%type<nodo> functionArgumentElements
%type<nodo> command
%type<nodo> commandSimple
%type<nodo> localVarDeclaration
%type<nodo> localVarCompleteDeclaration
%type<nodo> localVarInit
%type<nodo> attribution
%type<nodo> primitiveAttribution
%type<nodo> userTypeAttribution
%type<nodo> simpleExpression
%type<nodo> expression
%type<nodo> lowPrecedenceTwoFoldRecursiveExpression
%type<nodo> mediumPrecedenceTwoFoldRecursiveExpression
%type<nodo> highPrecedenceTwoFoldRecursiveExpression
%type<nodo> oneFoldRecursiveExpression
%type<nodo> operator
%type<nodo> unaryOperator
%type<nodo> unarySimbol
%type<nodo> arithmeticOperator
%type<nodo> precedentArithmeticOperator
%type<nodo> comparisonOperator
%type<nodo> expressionList
%type<nodo> functionCall
%type<nodo> functionCallArguments
%type<nodo> functionCallArgumentsList
%type<nodo> functionCallArgument
%type<nodo> shiftCommand
%type<nodo> fluxControlCommand
%type<nodo> conditionalFluxControl
%type<nodo> iterativeFluxControl
%type<nodo> selectionFluxControl
%type<nodo> commandSimpleList
%type<nodo> pipeCommands



//%type<nodo> test

%%

programa:
      code   {arvore = createChildren(createNode(AST_PROGRAMA), $1);};
    | %empty {arvore = createNode(AST_PROGRAMA);};

/* test:
    TK_IDENTIFICADOR TK_IDENTIFICADOR ';'
    
     {
       $$ = createNode(AST_ACCESSMOD); 
       createChildren($$, $1);
       createChildren($$, $2); 
       createChildren($$, $3); 
     };
 */
/*obvious stuff */

literal:
      TK_LIT_CHAR   {$$ = createNode(AST_LITERAL); createChildren($$, $1);}
    | TK_LIT_FALSE  {$$ = createNode(AST_LITERAL); createChildren($$, $1);}
    | TK_LIT_FLOAT  {$$ = createNode(AST_LITERAL); createChildren($$, $1);}
    | TK_LIT_INT    {$$ = createNode(AST_LITERAL); createChildren($$, $1);}
    | TK_LIT_STRING {$$ = createNode(AST_LITERAL); createChildren($$, $1);} 
    | TK_LIT_TRUE   {$$ = createNode(AST_LITERAL); createChildren($$, $1);};

pipe:
      TK_OC_BASH_PIPE    {$$ = createNode(AST_PIPE); createChildren($$, $1);}
    | TK_OC_FORWARD_PIPE {$$ = createNode(AST_PIPE); createChildren($$, $1);};

userVariableOrLiteral:
      TK_IDENTIFICADOR  {$$ = createNode(AST_USERVARORLIT); createChildren($$, $1);} 
    | literal           {$$ = createNode(AST_USERVARORLIT); createChildren($$, $1);};

type:
      TK_PR_INT         {$$ = createNode(AST_TYPE); createChildren($$, $1);}
    | TK_PR_FLOAT       {$$ = createNode(AST_TYPE); createChildren($$, $1);}
    | TK_PR_CHAR        {$$ = createNode(AST_TYPE); createChildren($$, $1);}
    | TK_PR_BOOL        {$$ = createNode(AST_TYPE); createChildren($$, $1);}
    | TK_PR_STRING      {$$ = createNode(AST_TYPE); createChildren($$, $1);}
    | TK_IDENTIFICADOR  {$$ = createNode(AST_TYPE); createChildren($$, $1);}; 

//Available types for class declaration
primitiveType:
      TK_PR_INT     {$$ = createNode(AST_PRIMITIVETYPE); createChildren($$, $1);}
    | TK_PR_FLOAT   {$$ = createNode(AST_PRIMITIVETYPE); createChildren($$, $1);}
    | TK_PR_CHAR    {$$ = createNode(AST_PRIMITIVETYPE); createChildren($$, $1);}
    | TK_PR_BOOL    {$$ = createNode(AST_PRIMITIVETYPE); createChildren($$, $1);}
    | TK_PR_STRING  {$$ = createNode(AST_PRIMITIVETYPE); createChildren($$, $1);};

shift:
      TK_OC_SR  {$$ = createNode(AST_SHIFT); createChildren($$, $1);}
    | TK_OC_SL  {$$ = createNode(AST_SHIFT); createChildren($$, $1);};
//Modifiers are usually optional
accessModifiers:
      TK_PR_PRIVATE     {$$ = createNode(AST_ACCESSMOD); createChildren($$, $1);}
    | TK_PR_PUBLIC      {$$ = createNode(AST_ACCESSMOD); createChildren($$, $1);}
    | TK_PR_PROTECTED   {$$ = createNode(AST_ACCESSMOD); createChildren($$, $1);}
    | %empty            {$$ = createNode(AST_ACCESSMOD);};

constModifier:
      TK_PR_CONST   {$$ = createNode(AST_CONSTMOD); createChildren($$, $1);}
    | %empty        {$$ = createNode(AST_CONSTMOD);};

vectorModifier:
     '[' expression ']' vectorModifier  {$$ = createNode(AST_VECTORMOD); 
                                         createChildren($$, $1); createChildren($$, $2);
                                         createChildren($$, $3); createChildren($$, $4);}
    | %empty                            {$$ = createNode(AST_VECTORMOD);};

vectorList:
      '[' expression ']'            {$$ = createNode(AST_VECTORLIST); 
                                         createChildren($$, $1); createChildren($$, $2);
                                         createChildren($$, $3);}
    | vectorList '[' expression ']' {$$ = createNode(AST_VECTORLIST); 
                                         createChildren($$, $1); createChildren($$, $2);
                                         createChildren($$, $3); createChildren($$, $4);};
/*obvious stuff end */

code:
      declaration       {$$ = createNode(AST_CODE); createChildren($$, $1);}
    | declaration code  {$$ = createNode(AST_CODE); createChildren($$, $1); createChildren($$, $2);};

declaration:
      classDeclaration      {$$ = createNode(AST_DECLARATION); createChildren($$, $1);}
    | globalVarDeclaration  {$$ = createNode(AST_DECLARATION); createChildren($$, $1);}
    | functionDeclaration   {$$ = createNode(AST_DECLARATION); createChildren($$, $1);}; 



/* A class is a declaration of a new type in the format:
    class id1 {
        protected int id2: 
        protected float id3
    }; 
*/
classDeclaration:
    accessModifiers TK_PR_CLASS TK_IDENTIFICADOR '[' fields ']' ';'
    {$$ = createNode(AST_CLASSDEC); 
     createChildren($$, $1); createChildren($$, $2);
     createChildren($$, $3); createChildren($$, $4);
     createChildren($$, $5); createChildren($$, $6);
     createChildren($$, $7);}; 

// fields are type and id inside classes that can't contain user-classes nor initialization of values
fields:
      accessModifiers primitiveType TK_IDENTIFICADOR            {$$ = createNode(AST_FIELDS); 
                                                                 createChildren($$, $1); createChildren($$, $2);
                                                                 createChildren($$, $3);}
    | accessModifiers primitiveType TK_IDENTIFICADOR ':' fields {$$ = createNode(AST_FIELDS); 
                                                                 createChildren($$, $1); createChildren($$, $2);
                                                                 createChildren($$, $3); createChildren($$, $4);
                                                                 createChildren($$, $5);}; 

//Declaration of global variables
globalVarDeclaration:
    TK_IDENTIFICADOR type  ';'                                 {$$ = createNode(AST_GLOBALVARDEC); 
                                                                createChildren($$, $1); createChildren($$, $2);
                                                                createChildren($$, $3);}
    | TK_IDENTIFICADOR TK_PR_STATIC type  ';'                  {$$ = createNode(AST_GLOBALVARDEC); 
                                                                createChildren($$, $1); createChildren($$, $2);
                                                                createChildren($$, $3); createChildren($$, $4);}
    | TK_IDENTIFICADOR '['TK_LIT_INT']' type  ';'              {$$ = createNode(AST_GLOBALVARDEC); 
                                                                createChildren($$, $1); createChildren($$, $2);
                                                                createChildren($$, $3); createChildren($$, $4);
                                                                createChildren($$, $5); createChildren($$, $6);}
    | TK_IDENTIFICADOR '['TK_LIT_INT']' TK_PR_STATIC type  ';' {$$ = createNode(AST_GLOBALVARDEC); 
                                                                createChildren($$, $1); createChildren($$, $2);
                                                                createChildren($$, $3); createChildren($$, $4);
                                                                createChildren($$, $5); createChildren($$, $6);
                                                                createChildren($$, $7);}; 

//Function declaration
functionDeclaration:
    functionHead commandsBlock {$$ = createNode(AST_FUNCDEC); createChildren($$, $1); createChildren($$, $2);};

functionHead:
      primitiveType TK_IDENTIFICADOR '(' functionArgumentsList ')'               {$$ = createNode(AST_FUNCHEAD); 
                                                                                  createChildren($$, $1); createChildren($$, $2);
                                                                                  createChildren($$, $3); createChildren($$, $4);
                                                                                  createChildren($$, $5);}
    | TK_IDENTIFICADOR TK_IDENTIFICADOR '(' functionArgumentsList ')'            {$$ = createNode(AST_FUNCHEAD); 
                                                                                  createChildren($$, $1); createChildren($$, $2);
                                                                                  createChildren($$, $3); createChildren($$, $4);
                                                                                  createChildren($$, $5);}
    | TK_PR_STATIC primitiveType TK_IDENTIFICADOR  '(' functionArgumentsList ')' {$$ = createNode(AST_FUNCHEAD); 
                                                                                  createChildren($$, $1); createChildren($$, $2);
                                                                                  createChildren($$, $3); createChildren($$, $4);
                                                                                  createChildren($$, $5); createChildren($$, $6);};

commandsBlock:
    '{' commandsList '}'    {$$ = createNode(AST_COMMANDSBLOCK); 
                             createChildren($$, $1); createChildren($$, $2);
                             createChildren($$, $3);}; 

commandsList:
      command commandsList  {$$ = createNode(AST_COMMANDSLIST); createChildren($$, $1); createChildren($$, $2);}
    | %empty                {$$ = createNode(AST_COMMANDSLIST);};

functionArgumentsList:
      functionArgumentElements  {$$ = createNode(AST_FUNCARGLIST); createChildren($$, $1);}
    | %empty                    {$$ = createNode(AST_FUNCARGLIST);};

functionArgumentElements:
      constModifier type TK_IDENTIFICADOR                              {$$ = createNode(AST_FUNCARGELEM); 
                                                                        createChildren($$, $1); createChildren($$, $2);
                                                                        createChildren($$, $3);}
    | constModifier type TK_IDENTIFICADOR ',' functionArgumentElements {$$ = createNode(AST_FUNCARGELEM); 
                                                                        createChildren($$, $1); createChildren($$, $2);
                                                                        createChildren($$, $3); createChildren($$, $4);
                                                                        createChildren($$, $5);};

//All Commands, it can be anypart of a simple command or it can be a case/output
command:
      commandSimple ';'              //All simple commands finish with a ';' 
                                     {$$ = createNode(AST_COMMAND); createChildren($$, $1); createChildren($$, $2);}
    | TK_PR_OUTPUT expressionList ';'//Output has parenthesis and thus is not a simple command
                                     {$$ = createNode(AST_COMMAND); 
                                           createChildren($$, $1); createChildren($$, $2);
                                           createChildren($$, $3);}
    | TK_PR_CASE TK_LIT_INT ':'      //case from switch cannot end in ';'
                                     {$$ = createNode(AST_COMMAND); 
                                           createChildren($$, $1); createChildren($$, $2);
                                           createChildren($$, $3);};    


//Commands without commas or case: Used in the "for" command
commandSimple:
      localVarCompleteDeclaration   {$$ = createNode(AST_COMMANDSIMPLE); createChildren($$, $1);}
    | attribution                   {$$ = createNode(AST_COMMANDSIMPLE); createChildren($$, $1);}
    | TK_PR_INPUT expression        {$$ = createNode(AST_COMMANDSIMPLE); createChildren($$, $1); createChildren($$, $2);}
    | functionCall                  {$$ = createNode(AST_COMMANDSIMPLE); createChildren($$, $1);}
    | shiftCommand                  {$$ = createNode(AST_COMMANDSIMPLE); createChildren($$, $1);}
    | TK_PR_RETURN expression       {$$ = createNode(AST_COMMANDSIMPLE); createChildren($$, $1); createChildren($$, $2);}
    | TK_PR_CONTINUE                {$$ = createNode(AST_COMMANDSIMPLE); createChildren($$, $1);}
    | TK_PR_BREAK                   {$$ = createNode(AST_COMMANDSIMPLE); createChildren($$, $1);}
    | fluxControlCommand            {$$ = createNode(AST_COMMANDSIMPLE); createChildren($$, $1);}
    | pipeCommands                  {$$ = createNode(AST_COMMANDSIMPLE); createChildren($$, $1);}
    | commandsBlock                 {$$ = createNode(AST_COMMANDSIMPLE); createChildren($$, $1);};      

//A variable declaration can be initialized ONLY if it has a primitive type
localVarDeclaration:
      primitiveType TK_IDENTIFICADOR localVarInit //If it starts with ID and is initialized 
                                                    {$$ = createNode(AST_LOCALVARDEC); 
                                                     createChildren($$, $1); createChildren($$, $2);
                                                     createChildren($$, $3);} 
    | TK_IDENTIFICADOR TK_IDENTIFICADOR          //userType userVariable
                                                    {$$ = createNode(AST_LOCALVARDEC); createChildren($$, $1); createChildren($$, $2);};
localVarCompleteDeclaration:
      TK_PR_STATIC TK_PR_CONST localVarDeclaration {$$ = createNode(AST_LOCALVARCDEC); 
                                                     createChildren($$, $1); createChildren($$, $2);
                                                     createChildren($$, $3);} 
    | TK_PR_STATIC localVarDeclaration             {$$ = createNode(AST_LOCALVARCDEC); createChildren($$, $1); createChildren($$, $2);}
    | TK_PR_CONST localVarDeclaration              {$$ = createNode(AST_LOCALVARCDEC); createChildren($$, $1); createChildren($$, $2);}
    | localVarDeclaration                          {$$ = createNode(AST_LOCALVARCDEC); createChildren($$, $1);};
    
//Initialization of variable
localVarInit:
      TK_OC_LE userVariableOrLiteral {$$ = createNode(AST_LOCALVARINIT); createChildren($$, $1); createChildren($$, $2);}
    | %empty                         {$$ = createNode(AST_LOCALVARINIT);};

attribution:
      primitiveAttribution  {$$ = createNode(AST_ATTRIBUTION); createChildren($$, $1);}
    | userTypeAttribution   {$$ = createNode(AST_ATTRIBUTION); createChildren($$, $1);};

primitiveAttribution:
      TK_IDENTIFICADOR vectorModifier '=' expression {$$ = createNode(AST_PRIMATTR); 
                                                      createChildren($$, $1); createChildren($$, $2);
                                                      createChildren($$, $3); createChildren($$, $4);};  
 //userVariable

userTypeAttribution:
      TK_IDENTIFICADOR vectorModifier '$' TK_IDENTIFICADOR '=' expression {$$ = createNode(AST_USERTYPEATTR); 
                                                                                  createChildren($$, $1); createChildren($$, $2);
                                                                                  createChildren($$, $3); createChildren($$, $4);
                                                                                  createChildren($$, $5); createChildren($$, $6);};

simpleExpression:
      TK_IDENTIFICADOR                          {$$ = createNode(AST_SIMPLEEXP); createChildren($$, $1);}
    | functionCall                              {$$ = createNode(AST_SIMPLEEXP); createChildren($$, $1);}
    | pipeCommands                              {$$ = createNode(AST_SIMPLEEXP); createChildren($$, $1);}
    | literal                                   {$$ = createNode(AST_SIMPLEEXP); createChildren($$, $1);}
    | TK_IDENTIFICADOR  '$' TK_IDENTIFICADOR    {$$ = createNode(AST_SIMPLEEXP); 
                                                      createChildren($$, $1); createChildren($$, $2);
                                                      createChildren($$, $3);};

expression:
    lowPrecedenceTwoFoldRecursiveExpression                                 {$$ = createNode(AST_EXPRESSION); createChildren($$, $1);}
    | lowPrecedenceTwoFoldRecursiveExpression '?' expression ':' expression {$$ = createNode(AST_EXPRESSION); 
                                                                                  createChildren($$, $1); createChildren($$, $2);
                                                                                  createChildren($$, $3); createChildren($$, $4);
                                                                                  createChildren($$, $5);};

lowPrecedenceTwoFoldRecursiveExpression:
    mediumPrecedenceTwoFoldRecursiveExpression                                                    {$$ = createNode(AST_LOWPTFREXP); createChildren($$, $1);}
    | lowPrecedenceTwoFoldRecursiveExpression operator mediumPrecedenceTwoFoldRecursiveExpression {$$ = createNode(AST_LOWPTFREXP); 
                                                                                                   createChildren($$, $1); createChildren($$, $2);
                                                                                                   createChildren($$, $3);}; 

mediumPrecedenceTwoFoldRecursiveExpression:
    highPrecedenceTwoFoldRecursiveExpression                                                                          {$$ = createNode(AST_MEDPTFREXP); createChildren($$, $1);}
    | mediumPrecedenceTwoFoldRecursiveExpression precedentArithmeticOperator highPrecedenceTwoFoldRecursiveExpression {$$ = createNode(AST_MEDPTFREXP); 
                                                                                                                       createChildren($$, $1); createChildren($$, $2);
                                                                                                                       createChildren($$, $3);}; 

highPrecedenceTwoFoldRecursiveExpression:
    oneFoldRecursiveExpression                                                {$$ = createNode(AST_HIGHPTFREXP); createChildren($$, $1);}
    | highPrecedenceTwoFoldRecursiveExpression '^' oneFoldRecursiveExpression {$$ = createNode(AST_HIGHPTFREXP); 
                                                                               createChildren($$, $1); createChildren($$, $2);
                                                                               createChildren($$, $3);}; 

oneFoldRecursiveExpression:
     simpleExpression                                                {$$ = createNode(AST_ONEFREXP); createChildren($$, $1);}
    | unaryOperator simpleExpression                                 {$$ = createNode(AST_ONEFREXP); createChildren($$, $1); createChildren($$, $2);}

    | '(' expression ')'                                             {$$ = createNode(AST_ONEFREXP); 
                                                                      createChildren($$, $1); createChildren($$, $2);
                                                                      createChildren($$, $3);}
    | unaryOperator '(' expression ')'                               {$$ = createNode(AST_ONEFREXP); 
                                                                      createChildren($$, $1); createChildren($$, $2);
                                                                      createChildren($$, $3); createChildren($$, $4);}

    | TK_IDENTIFICADOR vectorList                                    {$$ = createNode(AST_ONEFREXP); createChildren($$, $1); createChildren($$, $2);}
    | unaryOperator TK_IDENTIFICADOR vectorList                      {$$ = createNode(AST_ONEFREXP); 
                                                                      createChildren($$, $1); createChildren($$, $2);
                                                                      createChildren($$, $3);}

    | TK_IDENTIFICADOR vectorList '$' TK_IDENTIFICADOR               {$$ = createNode(AST_ONEFREXP); 
                                                                      createChildren($$, $1); createChildren($$, $2);
                                                                      createChildren($$, $3); createChildren($$, $4);}
    | unaryOperator TK_IDENTIFICADOR vectorList '$' TK_IDENTIFICADOR {$$ = createNode(AST_ONEFREXP); 
                                                                      createChildren($$, $1); createChildren($$, $2);
                                                                      createChildren($$, $3); createChildren($$, $4);
                                                                      createChildren($$, $5);};

operator:
      arithmeticOperator            {$$ = createNode(AST_OPERATOR); createChildren($$, $1);}
    | comparisonOperator            {$$ = createNode(AST_OPERATOR); createChildren($$, $1);};

unaryOperator:
       unarySimbol                  {$$ = createNode(AST_UNARYOPERATOR); createChildren($$, $1);}
    |  unaryOperator unarySimbol    {$$ = createNode(AST_UNARYOPERATOR); createChildren($$, $1); createChildren($$, $2);};

unarySimbol:
      '+'   {$$ = createNode(AST_UNARYSIMBOL); createChildren($$, $1);}
    | '-'   {$$ = createNode(AST_UNARYSIMBOL); createChildren($$, $1);}
    | '!'   {$$ = createNode(AST_UNARYSIMBOL); createChildren($$, $1);}   
    | '&'   {$$ = createNode(AST_UNARYSIMBOL); createChildren($$, $1);}
    | '*'   {$$ = createNode(AST_UNARYSIMBOL); createChildren($$, $1);}
    | '?'   {$$ = createNode(AST_UNARYSIMBOL); createChildren($$, $1);}
    | '#'   {$$ = createNode(AST_UNARYSIMBOL); createChildren($$, $1);};

arithmeticOperator:
      '+'{$$ = createNode(AST_ARITHMETICOP); createChildren($$, $1);} 
    | '-'{$$ = createNode(AST_ARITHMETICOP); createChildren($$, $1);}
    | '|'{$$ = createNode(AST_ARITHMETICOP); createChildren($$, $1);}
    | '&'{$$ = createNode(AST_ARITHMETICOP); createChildren($$, $1);};

precedentArithmeticOperator:
      '*'{$$ = createNode(AST_PRECARITHOP); createChildren($$, $1);}
    | '/'{$$ = createNode(AST_PRECARITHOP); createChildren($$, $1);}
    | '%'{$$ = createNode(AST_PRECARITHOP); createChildren($$, $1);};

comparisonOperator:
      TK_OC_LE  {$$ = createNode(AST_COMPARISONOP); createChildren($$, $1);}
    | TK_OC_GE  {$$ = createNode(AST_COMPARISONOP); createChildren($$, $1);}
    | TK_OC_EQ  {$$ = createNode(AST_COMPARISONOP); createChildren($$, $1);}
    | TK_OC_NE  {$$ = createNode(AST_COMPARISONOP); createChildren($$, $1);}
    | TK_OC_AND {$$ = createNode(AST_COMPARISONOP); createChildren($$, $1);}
    | TK_OC_OR  {$$ = createNode(AST_COMPARISONOP); createChildren($$, $1);}
    | '<'       {$$ = createNode(AST_COMPARISONOP); createChildren($$, $1);}
    | '>'       {$$ = createNode(AST_COMPARISONOP); createChildren($$, $1);};


expressionList:
      expression                     {$$ = createNode(AST_EXPLIST); createChildren($$, $1);}
    | expression ',' expressionList  {$$ = createNode(AST_EXPLIST); 
                                      createChildren($$, $1); createChildren($$, $2);
                                      createChildren($$, $3);}; 

//Function call has a very straight forward approach
functionCall:
    TK_IDENTIFICADOR '(' functionCallArguments ')' {$$ = createNode(AST_FUNCTIONCALL); 
                                                    createChildren($$, $1); createChildren($$, $2);
                                                    createChildren($$, $3); createChildren($$, $4);};

//Arguments can be empty or can be a list of expressions/dots
functionCallArguments:
      functionCallArgumentsList {$$ = createNode(AST_FUNCCALLARGS); createChildren($$, $1);}
    | %empty                    {$$ = createNode(AST_FUNCCALLARGS);}; 

//List of Expression/Dots
functionCallArgumentsList:
    functionCallArgument        {$$ = createNode(AST_FUNCARGLIST); createChildren($$, $1);}
    | functionCallArgument ',' functionCallArgumentsList {$$ = createNode(AST_FUNCARGLIST); 
                                                          createChildren($$, $1); createChildren($$, $2);
                                                          createChildren($$, $3);}; 
    
//Argument can be expression or dot
functionCallArgument:
      '.'           {$$ = createNode(AST_FUNCCALLARG); createChildren($$, $1);}
    | expression    {$$ = createNode(AST_FUNCCALLARG); createChildren($$, $1);};

//Shift command is straightforward too
shiftCommand:
      TK_IDENTIFICADOR vectorModifier shift expression                      {$$ = createNode(AST_SHIFT); 
                                                                             createChildren($$, $1); createChildren($$, $2);
                                                                             createChildren($$, $3);}
    | TK_IDENTIFICADOR vectorModifier '$' TK_IDENTIFICADOR shift expression {$$ = createNode(AST_SHIFT); 
                                                                             createChildren($$, $1); createChildren($$, $2);
                                                                             createChildren($$, $3); createChildren($$, $4);
                                                                             createChildren($$, $5);};

//Flux Control can be of 3 kinds: Conditional, iterative or selection
fluxControlCommand:
      conditionalFluxControl {$$ = createNode(AST_FLUXCONTCMD); createChildren($$, $1);}
    | iterativeFluxControl   {$$ = createNode(AST_FLUXCONTCMD); createChildren($$, $1);}
    | selectionFluxControl   {$$ = createNode(AST_FLUXCONTCMD); createChildren($$, $1);};

//Conditional flux is if (exp) then {...} else {...}, with the else being optional
conditionalFluxControl:
      TK_PR_IF '(' expression ')' TK_PR_THEN commandsBlock                          {$$ = createNode(AST_CONDFLUXCONT); 
                                                                                     createChildren($$, $1); createChildren($$, $2);
                                                                                     createChildren($$, $3); createChildren($$, $4);
                                                                                     createChildren($$, $5); createChildren($$, $6);}
    | TK_PR_IF '(' expression ')' TK_PR_THEN commandsBlock TK_PR_ELSE commandsBlock {$$ = createNode(AST_CONDFLUXCONT); 
                                                                                     createChildren($$, $1); createChildren($$, $2);
                                                                                     createChildren($$, $3); createChildren($$, $4);
                                                                                     createChildren($$, $5); createChildren($$, $6);
                                                                                     createChildren($$, $7); createChildren($$, $8);};

//There are 4 variations of iterative flux control
iterativeFluxControl:
      TK_PR_FOREACH '(' TK_IDENTIFICADOR ':' expressionList ')' commandsBlock               {$$ = createNode(AST_CONDFLUXCONT); 
                                                                                                    createChildren($$, $1); createChildren($$, $2);
                                                                                                    createChildren($$, $3); createChildren($$, $4);
                                                                                                    createChildren($$, $5); createChildren($$, $6);
                                                                                                    createChildren($$, $7);}
    | TK_PR_FOR '(' commandSimpleList ':' expression ':' commandSimpleList ')' commandsBlock //The command list is of simple commands
                                                                                            {$$ = createNode(AST_CONDFLUXCONT); 
                                                                                             createChildren($$, $1); createChildren($$, $2);
                                                                                             createChildren($$, $3); createChildren($$, $4);
                                                                                             createChildren($$, $5); createChildren($$, $6);
                                                                                             createChildren($$, $7); createChildren($$, $8);
                                                                                             createChildren($$, $9);}
    | TK_PR_WHILE '(' expression ')' TK_PR_DO commandsBlock                                 {$$ = createNode(AST_CONDFLUXCONT); 
                                                                                             createChildren($$, $1); createChildren($$, $2);
                                                                                             createChildren($$, $3); createChildren($$, $4);
                                                                                             createChildren($$, $5); createChildren($$, $6);}
    | TK_PR_DO commandsBlock TK_PR_WHILE '(' expression ')'                                 {$$ = createNode(AST_CONDFLUXCONT); 
                                                                                             createChildren($$, $1); createChildren($$, $2);
                                                                                             createChildren($$, $3); createChildren($$, $4);
                                                                                             createChildren($$, $5); createChildren($$, $6);};      

//The only selection flux control is switch
//It is important to note that bison won't check if there are cases on the commandsBlock
selectionFluxControl:
    TK_PR_SWITCH '(' expression ')' commandsBlock {$$ = createNode(AST_SELECFLUXCONT); 
                                                   createChildren($$, $1); createChildren($$, $2);
                                                   createChildren($$, $3); createChildren($$, $4);
                                                   createChildren($$, $5);}; 

//List of commands without commas in them, used in for
commandSimpleList:
      commandSimple                       {$$ = createNode(AST_COMMANDSIMPLELIST); createChildren($$, $1);}
    | commandSimpleList ',' commandSimple {$$ = createNode(AST_COMMANDSIMPLELIST); 
                                           createChildren($$, $1); createChildren($$, $2);
                                           createChildren($$, $3);};

//Pipe Commands can be in the format of "f() %>% f()" or "f() %>% f() %>% ... f()"
pipeCommands:
      functionCall pipe functionCall {$$ = createNode(AST_PIPECOMMANDS); 
                                      createChildren($$, $1); createChildren($$, $2);
                                      createChildren($$, $3);}
    | pipeCommands pipe functionCall {$$ = createNode(AST_PIPECOMMANDS); 
                                      createChildren($$, $1); createChildren($$, $2);
                                      createChildren($$, $3);};

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
    cleanList(nodeList);
    nodeList = NULL;
    arvore = NULL;
}