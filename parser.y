/*INF UFRGS 2018/2// COMPILADORES SCHNORR // GRUPO KAPPA // GUILHERME CATTANI DE CASTRO 00243589 && CAIO F RODRIGUES 00261578*/
%code requires { 
    #include "auxiliar.h"

    #include <stdio.h>
    #include <string.h>
    #include "ASTree.h"
    #include "symbolTableInterface.h"

    int yylex(void);
    void yyerror (char const *s);
    
    extern int yylineno;
    extern char* previous_text;
    extern int yycolno;
    extern char* yytext;
    extern void* arvore;
    extern struct nodeList* nodeList;

    void semanticerror(int err, struct node* id, struct node* type);
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
%type<nodo> functionCommandsBlock
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
    {createContext();}  code   {arvore = createChildren(createNode(AST_PROGRAMA), $2); deleteContext();};
                      | %empty {arvore = createNode(AST_PROGRAMA);};


literal:
      TK_LIT_CHAR   {$$ = $1;}
    | TK_LIT_FALSE  {$$ = $1;}
    | TK_LIT_FLOAT  {$$ = $1;}
    | TK_LIT_INT    {$$ = $1;}
    | TK_LIT_STRING {$$ = $1;} 
    | TK_LIT_TRUE   {$$ = $1;};

pipe:
      TK_OC_BASH_PIPE    {$$ = $1;}
    | TK_OC_FORWARD_PIPE {$$ = $1;};

userVariableOrLiteral:
      TK_IDENTIFICADOR  {$$ = $1;} 
    | literal           {$$ = $1;};

type:
      TK_PR_INT         {$$ = $1;}
    | TK_PR_FLOAT       {$$ = $1;}
    | TK_PR_CHAR        {$$ = $1;}
    | TK_PR_BOOL        {$$ = $1;}
    | TK_PR_STRING      {$$ = $1;}
    | TK_IDENTIFICADOR  {$$ = $1;}; 

//Available types for class declaration
primitiveType:
      TK_PR_INT     {$$ = $1;}
    | TK_PR_FLOAT   {$$ = $1;}
    | TK_PR_CHAR    {$$ = $1;}
    | TK_PR_BOOL    {$$ = $1;}
    | TK_PR_STRING  {$$ = $1;};

shift:
      TK_OC_SR  {$$ = $1;}
    | TK_OC_SL  {$$ = $1;};
//Modifiers are usually optional
accessModifiers:
      TK_PR_PRIVATE     {$$ = $1;}
    | TK_PR_PUBLIC      {$$ = $1;}
    | TK_PR_PROTECTED   {$$ = $1;}
    | %empty            {$$ = createNode(AST_ACCESSMOD);};

constModifier:
      TK_PR_CONST   {$$ = $1;}
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
      declaration       {$$ = $1;}
    | declaration code  {$$ = createNode(AST_CODE); createChildren($$, $1); createChildren($$, $2);};

declaration:
      classDeclaration      {$$ = $1;}
    | globalVarDeclaration  {$$ = $1;}
    | functionDeclaration   {$$ = $1;}; 



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
     createChildren($$, $7);
     int err = addSymbolFromNodeClass($3, $5);
     if (err) { semanticerror(err, $3, NULL); return err; } };

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
                                                                createChildren($$, $3);
                                                                int err = addSymbolFromNode($1,$2);
                                                                if (err) { semanticerror(err, $1, $2); return err; } 
                                                               }
    | TK_IDENTIFICADOR TK_PR_STATIC type  ';'                  {$$ = createNode(AST_GLOBALVARDEC); 
                                                                createChildren($$, $1); createChildren($$, $2);
                                                                createChildren($$, $3); createChildren($$, $4);
                                                                int err = addSymbolFromNode($1,$3);
                                                                if (err) { semanticerror(err, $1, $3); return err; }
                                                               }
    | TK_IDENTIFICADOR '['TK_LIT_INT']' type  ';'              {$$ = createNode(AST_GLOBALVARDEC); 
                                                                createChildren($$, $1); createChildren($$, $2);
                                                                createChildren($$, $3); createChildren($$, $4);
                                                                createChildren($$, $5); createChildren($$, $6);
                                                                int err = addSymbolFromNodeWithVector($1,$5,$3);
                                                                if (err) { semanticerror(err, $1, $5); return err; }
                                                               }
    | TK_IDENTIFICADOR '['TK_LIT_INT']' TK_PR_STATIC type  ';' {$$ = createNode(AST_GLOBALVARDEC); 
                                                                createChildren($$, $1); createChildren($$, $2);
                                                                createChildren($$, $3); createChildren($$, $4);
                                                                createChildren($$, $5); createChildren($$, $6);
                                                                createChildren($$, $7);
                                                                int err = addSymbolFromNodeWithVector($1,$6,$3);
                                                                if (err) { semanticerror(err, $1, $6); return err; }
                                                                }; 

//Function declaration
functionDeclaration:
    functionHead {createContext(); 
                    int err = addSymbolFromNodeFunction($1);
                    if (err && numberOfChildren($1) == 5 ) { 
                       semanticerror(err, $1->child->brother, $1->child); 
                       return err;
                    }
                    else if (err && numberOfChildren($1) == 6){
                       semanticerror(err, $1->child->brother->brother, $1->child->brother);
                       return err;
                    };                    
                } 
    functionCommandsBlock                           { deleteContext();
                                                    $$ = createNode(AST_FUNCDEC);
                                                    createChildren($$, $1);
                                                    createChildren($$, $3);
                                                    int err = addSymbolFromNodeFunction($1);
                                                    if (err && numberOfChildren($1) == 5 ) { 
                                                        semanticerror(err, $1->child->brother, $1->child); 
                                                        return err;
                                                    }
                                                   else if (err && numberOfChildren($1) == 6){
                                                       semanticerror(err, $1->child->brother->brother, $1->child->brother);
                                                       return err;
                                                   };};

functionCommandsBlock:
    '{' commandsList '}'    {$$ = createNode(AST_FUNCCOMMANDSBLOCK); 
                             createChildren($$, $1); createChildren($$, $2);
                             createChildren($$, $3);}; 

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
    { createContext(); }'{' commandsList '}'    {$$ = createNode(AST_COMMANDSBLOCK); 
                                                createChildren($$, $2); createChildren($$, $3);
                                                createChildren($$, $4);
                                                deleteContext();}; 

commandsList:
      command commandsList  {$$ = createNode(AST_COMMANDSLIST); createChildren($$, $1); createChildren($$, $2);}
    | %empty                {$$ = createNode(AST_COMMANDSLIST);};

functionArgumentsList:
      functionArgumentElements  {$$ = $1;}
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


//Commands without commas or case. Used in the "for" command
commandSimple:
      localVarCompleteDeclaration   {$$ = $1; int err = addSymbolFromLocalVarDeclaration($1);
                                    if (err) { 
                                        int numberOfChildrenInt = numberOfChildren($1);
                                        if (numberOfChildrenInt == 3 ) { 
                                            semanticerror(err, $1->child->brother->brother->child->brother, $1->child->brother->brother->child); 
                                        }
                                        else if(numberOfChildrenInt == 2){ 
                                            semanticerror(err, $1->child->brother->child->brother, $1->child->brother->child); 
                                        }
                                        else if(numberOfChildrenInt == 1){ 
                                            semanticerror(err, $1->child->child->brother, $1->child->child); 
                                        }
                                        return err;} 
                                    } 
    | attribution                                   {$$ = $1;}
    | TK_PR_INPUT expression                        {$$ = createNode(AST_COMMANDSIMPLE); createChildren($$, $1); createChildren($$, $2);
                                                    int err = calculateTypeInfer($2); if(err > 6){ semanticerror(err, $1,$1); return err;}}
    | functionCall                                  {$$ = $1;}
    | shiftCommand                                  {$$ = $1;}
    | TK_PR_RETURN expression                       {$$ = createNode(AST_COMMANDSIMPLE); createChildren($$, $1); createChildren($$, $2);
                                                    int err = calculateTypeInfer($2); if(err > 6){ semanticerror(err, $1,$1); return err;}}
    | TK_PR_CONTINUE                                {$$ = $1;}
    | TK_PR_BREAK                                   {$$ = $1;}
    | fluxControlCommand                            {$$ = $1;}
    | pipeCommands                                  {$$ = $1;}
    | commandsBlock                                 {$$ = $1;};       

//A variable declaration can be initialized ONLY if it has a primitive type
localVarDeclaration:
      primitiveType TK_IDENTIFICADOR localVarInit //If it starts with ID and is initialized 
                                                    {$$ = createNode(AST_LOCALVARDEC); 
                                                     createChildren($$, $1); createChildren($$, $2);
                                                     createChildren($$, $3);
                                                     } 
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
      primitiveAttribution  {$$ = $1; int err = checkPrimitiveAttribution($1);
                            if(err){ semanticerror(err, $1->child, NULL); return err;}}
    | userTypeAttribution   {$$ = $1; int err = checkUserTypeAttribution($1);
                            if(err){ semanticerror(err, $1->child, $1->child->brother->brother->brother); return err;}};

primitiveAttribution:
      TK_IDENTIFICADOR vectorModifier '=' expression {$$ = createNode(AST_PRIMATTR); 
                                                        createChildren($$, $1); createChildren($$, $2);
                                                        createChildren($$, $3); createChildren($$, $4);};  
 //userVariable

userTypeAttribution:
      TK_IDENTIFICADOR vectorModifier '$' TK_IDENTIFICADOR '=' expression {$$ = createNode(AST_USERTYPEATTR); 
                    
                                                                                createChildren($$, $1); createChildren($$, $2);
                                                                                createChildren($$, $3); createChildren($$, $4);
                                                                                createChildren($$, $5); createChildren($$, $6);
                                                                                
                                                                                if(!isIdentifierDeclared($1)){
                                                                                    semanticerror(ERR_UNDECLARED, $1, NULL); 
                                                                                    return ERR_UNDECLARED;}
                                                                                
                                                                                if(getTypeFromUserClassField($1, $4) > NATUREZA_IDENTIFICADOR){
                                                                                    semanticerror(ERR_CLASS_ID_NOT_FOUND, $1, NULL); 
                                                                                    return ERR_CLASS_ID_NOT_FOUND;}

                                                                                if(!isVectorEmpty($2) && !isIdentifierOfNatureClassVector($1) ){
                                                                                    semanticerror(ERR_VECTOR, $1, NULL); 
                                                                                    return ERR_VECTOR;}
                                                                                };

simpleExpression:
      TK_IDENTIFICADOR                          {$$ = $1;
                                                 if(!isIdentifierDeclared($1)){
                                                    semanticerror(ERR_UNDECLARED, $1, NULL); 
                                                    return ERR_UNDECLARED;}
                                                }
    | functionCall                              {$$ = $1;}
    | pipeCommands                              {$$ = $1;}
    | literal                                   {$$ = $1;}
    | TK_IDENTIFICADOR  '$' TK_IDENTIFICADOR    {$$ = createNode(AST_SIMPLEEXP); 
                                                      createChildren($$, $1); createChildren($$, $2);
                                                      createChildren($$, $3);
                                                 if(!isIdentifierDeclared($1)){
                                                    semanticerror(ERR_UNDECLARED, $1, NULL); 
                                                    return ERR_UNDECLARED;}

                                                 if(getTypeFromUserClassField($1, $3) > NATUREZA_IDENTIFICADOR){
                                                    semanticerror(ERR_CLASS_ID_NOT_FOUND, $1, NULL); 
                                                    return ERR_CLASS_ID_NOT_FOUND;}
                                                };

expression:
    lowPrecedenceTwoFoldRecursiveExpression                                 {$$ = $1;}
    | lowPrecedenceTwoFoldRecursiveExpression '?' expression ':' expression {$$ = createNode(AST_EXPRESSION); 
                                                                                  createChildren($$, $1); createChildren($$, $2);
                                                                                  createChildren($$, $3); createChildren($$, $4);
                                                                                  createChildren($$, $5);};

lowPrecedenceTwoFoldRecursiveExpression:
    mediumPrecedenceTwoFoldRecursiveExpression                                                    {$$ = $1;}
    | lowPrecedenceTwoFoldRecursiveExpression operator mediumPrecedenceTwoFoldRecursiveExpression {$$ = createNode(AST_LOWPTFREXP); 
                                                                                                   createChildren($$, $1); createChildren($$, $2);
                                                                                                   createChildren($$, $3);}; 

mediumPrecedenceTwoFoldRecursiveExpression:
    highPrecedenceTwoFoldRecursiveExpression                                                                          {$$ = $1;}
    | mediumPrecedenceTwoFoldRecursiveExpression precedentArithmeticOperator highPrecedenceTwoFoldRecursiveExpression {$$ = createNode(AST_MEDPTFREXP); 
                                                                                                                       createChildren($$, $1); createChildren($$, $2);
                                                                                                                       createChildren($$, $3);}; 

highPrecedenceTwoFoldRecursiveExpression:
    oneFoldRecursiveExpression                                                {$$ = $1;}
    | highPrecedenceTwoFoldRecursiveExpression '^' oneFoldRecursiveExpression {$$ = createNode(AST_HIGHPTFREXP); 
                                                                               createChildren($$, $1); createChildren($$, $2);
                                                                               createChildren($$, $3);}; 

oneFoldRecursiveExpression:
     simpleExpression                                                {$$ = $1;}
    | unaryOperator simpleExpression                                 {$$ = createNode(AST_ONEFREXP); createChildren($$, $1); createChildren($$, $2);}

    | '(' expression ')'                                             {$$ = createNode(AST_ONEFREXP); 
                                                                      createChildren($$, $1); createChildren($$, $2);
                                                                      createChildren($$, $3);}
    | unaryOperator '(' expression ')'                               {$$ = createNode(AST_ONEFREXP); 
                                                                      createChildren($$, $1); createChildren($$, $2);
                                                                      createChildren($$, $3); createChildren($$, $4);}

    | TK_IDENTIFICADOR vectorList                                    {$$ = createNode(AST_ONEFREXP); createChildren($$, $1); createChildren($$, $2);
                                                                      if(!isIdentifierDeclared($1)){
                                                                        semanticerror(ERR_UNDECLARED, $1, NULL); 
                                                                        return ERR_UNDECLARED;}

                                                                      if(!isVectorEmpty($2) && !isIdentifierOfNatureVector($1) ){
                                                                        semanticerror(ERR_VECTOR, $1, NULL); 
                                                                        return ERR_VECTOR;}
                                                                     }
    | unaryOperator TK_IDENTIFICADOR vectorList                      {$$ = createNode(AST_ONEFREXP); 
                                                                      createChildren($$, $1); createChildren($$, $2);
                                                                      createChildren($$, $3);
                                                                      if(!isIdentifierDeclared($2)){
                                                                        semanticerror(ERR_UNDECLARED, $1, NULL); 
                                                                        return ERR_UNDECLARED;}

                                                                      if(!isVectorEmpty($3) && !isIdentifierOfNatureVector($2) ){
                                                                        semanticerror(ERR_VECTOR, $2, NULL); 
                                                                        return ERR_VECTOR;}
                                                                     }

    | TK_IDENTIFICADOR vectorList '$' TK_IDENTIFICADOR               {$$ = createNode(AST_ONEFREXP); 
                                                                      createChildren($$, $1); createChildren($$, $2);
                                                                      createChildren($$, $3); createChildren($$, $4);
                                                                      if(!isIdentifierDeclared($1)){
                                                                        semanticerror(ERR_UNDECLARED, $1, NULL); 
                                                                        return ERR_UNDECLARED;}
                                                                    
                                                                      if(getTypeFromUserClassField($1, $4) > NATUREZA_IDENTIFICADOR){
                                                                        semanticerror(ERR_CLASS_ID_NOT_FOUND, $1, NULL); 
                                                                        return ERR_CLASS_ID_NOT_FOUND;}

                                                                      if(!isVectorEmpty($2) && !isIdentifierOfNatureClassVector($1) ){
                                                                        semanticerror(ERR_VECTOR, $1, NULL); 
                                                                        return ERR_VECTOR;}
                                                                    }
    | unaryOperator TK_IDENTIFICADOR vectorList '$' TK_IDENTIFICADOR {$$ = createNode(AST_ONEFREXP); 
                                                                      createChildren($$, $1); createChildren($$, $2);
                                                                      createChildren($$, $3); createChildren($$, $4);
                                                                      createChildren($$, $5);
                                                                      if(!isIdentifierDeclared($1)){
                                                                        semanticerror(ERR_UNDECLARED, $2, NULL); 
                                                                        return ERR_UNDECLARED;}

                                                                      if(getTypeFromUserClassField($2, $5) > NATUREZA_IDENTIFICADOR){
                                                                        semanticerror(ERR_CLASS_ID_NOT_FOUND, $2, NULL); 
                                                                        return ERR_CLASS_ID_NOT_FOUND;}

                                                                      if(!isVectorEmpty($3) && !isIdentifierOfNatureClassVector($2) ){
                                                                        semanticerror(ERR_VECTOR, $2, NULL); 
                                                                        return ERR_VECTOR;}
                                                                     };

operator:
      arithmeticOperator            {$$ = $1;}
    | comparisonOperator            {$$ = $1;};

unaryOperator:
       unarySimbol                  {$$ = $1;}
    |  unaryOperator unarySimbol    {$$ = createNode(AST_UNARYOPERATOR); createChildren($$, $1); createChildren($$, $2);};

unarySimbol:
      '+'   {$$ = $1;}
    | '-'   {$$ = $1;}
    | '!'   {$$ = $1;}   
    | '&'   {$$ = $1;}
    | '*'   {$$ = $1;}
    | '?'   {$$ = $1;}
    | '#'   {$$ = $1;};

arithmeticOperator:
      '+'{$$ = $1;} 
    | '-'{$$ = $1;}
    | '|'{$$ = $1;}
    | '&'{$$ = $1;};

precedentArithmeticOperator:
      '*'{$$ = $1;}
    | '/'{$$ = $1;}
    | '%'{$$ = $1;};

comparisonOperator:
      TK_OC_LE  {$$ = $1;}
    | TK_OC_GE  {$$ = $1;}
    | TK_OC_EQ  {$$ = $1;}
    | TK_OC_NE  {$$ = $1;}
    | TK_OC_AND {$$ = $1;}
    | TK_OC_OR  {$$ = $1;}
    | '<'       {$$ = $1;}
    | '>'       {$$ = $1;};


expressionList:
      expression                     {$$ = $1;}
    | expression ',' expressionList  {$$ = createNode(AST_EXPLIST); 
                                      createChildren($$, $1); createChildren($$, $2);
                                      createChildren($$, $3);}; 

//Function call has a very straight forward approach
functionCall:
    TK_IDENTIFICADOR '(' functionCallArguments ')' {$$ = createNode(AST_FUNCTIONCALL); 
                                                    createChildren($$, $1); createChildren($$, $2);
                                                    createChildren($$, $3); createChildren($$, $4);
                                                    int err = checkFunction($$); if (err){ semanticerror(err, $1, NULL); return err;}
                                                    };

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
      '.'           {$$ = $1;}
    | expression    {$$ = $1;};

//Shift command is straightforward too
shiftCommand:
      TK_IDENTIFICADOR vectorModifier shift expression                      {$$ = createNode(AST_SHIFT); 
                                                                             createChildren($$, $1); createChildren($$, $2);
                                                                             createChildren($$, $3); createChildren($$, $4);
                                                                             int err = checkPrimitiveAttribution($$);
                                                                             if(err){ semanticerror(err, $1, NULL); return err;}
                                                                            }
    | TK_IDENTIFICADOR vectorModifier '$' TK_IDENTIFICADOR shift expression {$$ = createNode(AST_SHIFT); 
                                                                             createChildren($$, $1); createChildren($$, $2);
                                                                             createChildren($$, $3); createChildren($$, $4);
                                                                             createChildren($$, $5); createChildren($$, $6);
                                                                             int err = checkUserTypeAttribution($$);
                                                                             if(err){ semanticerror(ERR_UNKNOWN, $1, $4); return err;}
                                                                            };

//Flux Control can be of 3 kinds, Conditional, iterative or selection
fluxControlCommand:
      conditionalFluxControl {$$ = $1;}
    | iterativeFluxControl   {$$ = $1;}
    | selectionFluxControl   {$$ = $1;};

//Conditional flux is if (exp) then {...} else {...}, with the else being optional
conditionalFluxControl:
      TK_PR_IF '(' expression ')' TK_PR_THEN commandsBlock                          {$$ = createNode(AST_CONDFLUXCONT); 
                                                                                     createChildren($$, $1); createChildren($$, $2);
                                                                                     createChildren($$, $3); createChildren($$, $4);
                                                                                     createChildren($$, $5); createChildren($$, $6);
                                                                                     int err = calculateTypeInfer($3); 
                                                                                     if(err > 6){ semanticerror(err, $1,$1); return err;}}
    | TK_PR_IF '(' expression ')' TK_PR_THEN commandsBlock TK_PR_ELSE commandsBlock {$$ = createNode(AST_CONDFLUXCONT); 
                                                                                     createChildren($$, $1); createChildren($$, $2);
                                                                                     createChildren($$, $3); createChildren($$, $4);
                                                                                     createChildren($$, $5); createChildren($$, $6);
                                                                                     createChildren($$, $7); createChildren($$, $8);
                                                                                     int err = calculateTypeInfer($3); 
                                                                                     if(err > 6){ semanticerror(err, $1,$1); return err;}};

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
                                                                                             createChildren($$, $5); createChildren($$, $6);
                                                                                             int err = calculateTypeInfer($3); 
                                                                                             if(err > 6){ semanticerror(err, $1,$1); return err;}}
    | TK_PR_DO commandsBlock TK_PR_WHILE '(' expression ')'                                 {$$ = createNode(AST_CONDFLUXCONT); 
                                                                                             createChildren($$, $1); createChildren($$, $2);
                                                                                             createChildren($$, $3); createChildren($$, $4);
                                                                                             createChildren($$, $5); createChildren($$, $6);
                                                                                             int err = calculateTypeInfer($5); 
                                                                                             if(err > 6){ semanticerror(err, $4,$4); return err;}};      

//The only selection flux control is switch
//It is important to note that bison won't check if there are cases on the commandsBlock
selectionFluxControl:
    TK_PR_SWITCH '(' expression ')' commandsBlock {$$ = createNode(AST_SELECFLUXCONT); 
                                                   createChildren($$, $1); createChildren($$, $2);
                                                   createChildren($$, $3); createChildren($$, $4);
                                                   createChildren($$, $5);
                                                   int err = calculateTypeInfer($3); 
                                                   if(err > 6){ semanticerror(err, $1,$1); return err;}}; 

//List of commands without commas in them, used in for
commandSimpleList:
      commandSimple                       {$$ = $1;}
    | commandSimpleList ',' commandSimple {$$ = createNode(AST_COMMANDSIMPLELIST); 
                                           createChildren($$, $1); createChildren($$, $2);
                                           createChildren($$, $3);};

//Pipe Commands can be in the format of "f() %>% f()" or "f() %>% f() %>% ... f()"
pipeCommands:
      functionCall pipe functionCall {$$ = createNode(AST_PIPECOMMANDS); 
                                      createChildren($$, $1); createChildren($$, $2);
                                      createChildren($$, $3);
                                      }
    | pipeCommands pipe functionCall {$$ = createNode(AST_PIPECOMMANDS); 
                                      createChildren($$, $1); createChildren($$, $2);
                                      createChildren($$, $3);};

%%


void yyerror (char const *s){
    free(previous_text);
    printf("Line %d, Column %d: %s near \"%s\"\n",yylineno, yycolno, s, yylval.nodo->token_value);  
    deleteAllContext();

}

void semanticerror(int err, struct node* id, struct node* type){
    switch(err){
        case ERR_UNDECLARED:
            printf ("Line %d, Column %d: Identifier \"%s\" not declared.\n", id->line_number, id->col_number, id->token_value);
            break;
        case ERR_DECLARED:
            printf ("Line %d, Column %d: Identifier \"%s\" already declared.\n", id->line_number, id->col_number, id->token_value);
            break;
        case ERR_VARIABLE:
            printf ("Line %d, Column %d: Identifier \"%s\" must be used without indexation.\n", id->line_number, id->col_number, id->token_value);
            break;
        case ERR_VECTOR:
            printf ("Line %d, Column %d: Identifier \"%s\" must be used with indexation.\n", id->line_number, id->col_number, id->token_value);
            break;
        case ERR_FUNCTION:
            printf ("Line %d, Column %d: Identifier \"%s\" must be used as function.\n", id->line_number, id->col_number, id->token_value);
            break;
        case ERR_USER:
            printf ("Line %d, Column %d: Identifier \"%s\" must be used as user class.\n", id->line_number, id->col_number, id->token_value);
            break;       
        case ERR_CLASS:
            printf ("Line %d, Column %d: Identifier \"%s\" must not be used as user class or function.\n", id->line_number, id->col_number, id->token_value);
            break;     
        case ERR_CLASS_ID_NOT_FOUND:
            if(type == NULL){
                printf ("Line %d, Column %d: Field not found on class \"%s\".\n", id->line_number, id->col_number, id->token_value);
                break;
            }
            printf ("Line %d, Column %d: Field \"%s\" of class \"%s\".\n", id->line_number, id->col_number, type->token_value, id->token_value);
            break;   
        case ERR_WRONG_TYPE:
            if(type == NULL){
                printf ("Line %d, Column %d: Type of attribution incompatible with \"%s\".\n", id->line_number, id->col_number, id->token_value);
                break;
            }
            printf ("Line %d, Column %d: Type \"%s\" incompatible with \"%s\".\n", id->line_number, id->col_number, id->token_value, type->token_value);
            break; 
        case ERR_STRING_TO_X:
            printf ("Line %d, Column %d: Identifier \"%s\" cannot be coerced from string.\n", id->line_number, id->col_number, id->token_value);
            break; 
        case ERR_CHAR_TO_X:
            printf ("Line %d, Column %d: Identifier \"%s\" cannot be coerced from char.\n", id->line_number, id->col_number, id->token_value);
            break; 
        case ERR_USER_TO_X:
            printf ("Line %d, Column %d: Identifier \"%s\" cannot be coerced from user class.\n", id->line_number, id->col_number, id->token_value);
            break; 
        case ERR_MISSING_ARGS:
            printf ("Line %d, Column %d: Identifier \"%s\" is missing arguments.\n", id->line_number, id->col_number, id->token_value);
            break; 
        case ERR_EXCESS_ARGS:
            printf ("Line %d, Column %d: Identifier \"%s\" is with excess arguments.\n", id->line_number, id->col_number, id->token_value);
            break;
        case ERR_WRONG_TYPE_ARGS:
            printf ("Line %d, Column %d: Identifier \"%s\" types are incompatible.\n", id->line_number, id->col_number, id->token_value);
            break;
        case ERR_WRONG_PAR_INPUT:
            printf ("Line %d, Column %d: Parameter \"%s\" is not identifier.\n", id->line_number, id->col_number, id->token_value);
            break;
        case ERR_WRONG_PAR_OUTPUT:
            printf ("Line %d, Column %d: Parameter \"%s\" is not literal string or expression.\n", id->line_number, id->col_number, id->token_value);
            break;
        case ERR_WRONG_PAR_RETURN:
            printf ("Line %d, Column %d: Parameter \"%s\" is not compatible with the type of return.\n", id->line_number, id->col_number, id->token_value);
            break;
        case ERR_TYPE_UNDECLARED:
            if(type == NULL){
                printf ("Line %d, Column %d: Type of identifier \"%s\" not declared.\n",id->line_number, id->col_number, id->token_value);
                break;
            }
            printf ("Line %d, Column %d: Type \"%s\" of identifier \"%s\" not declared.\n", id->line_number, id->col_number, type->token_value, id->token_value);
            break;
        case ERR_STR_SIZE_OVERFLOW:
            printf ("Line %d, Column %d: String \"%s\" is too small to hold the attribution.\n", id->line_number, id->col_number, id->token_value);
            break;
        case ERR_UNKNOWN:
            printf ("Line %d, Column %d: Unknown error.\n", yylineno, yycolno, id->token_value);
            break;
    }
    deleteAllContext();
}
void descompila (void *arvore){
    showTreeRecursion(arvore, 0);
}

void libera (void *arvore){
    cleanList(nodeList);
    nodeList = NULL;
    arvore = NULL;
}