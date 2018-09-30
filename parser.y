/*INF UFRGS 2018/2// COMPILADORES SCHNORR // GRUPO KAPPA // GUILHERME CATTANI DE CASTRO 00243589 && CAIO F RODRIGUES 00261578*/
%code requires { 
    #define FALSE             0
    #define TRUE              1

    #define RESERVED_WORD     2
    #define SPECIAL_CHARACTER 3
    #define COMPOUND_OPERATOR 4
    #define IDENTIFIER        5

    #define LITERAL_INT       6
    #define LITERAL_BOOL      7
    #define LITERAL_FLOAT     8
    #define LITERAL_CHAR      9
    #define LITERAL_STRING    10

    #define AST_PROGRAM              100
    #define AST_IDENTIFIER           101
    #define AST_IF_ELSE              102

    #include <stdio.h>
    #include <string.h>
    int yylex(void);
    void yyerror (char const *s);

    extern int yylineno;
    extern char* yytext;
    extern void* arvore;

    struct node{
        int     line_number;
        int     token_type;
        char*   token_value;

        int     int_value;
        int     bool_value;
        float   float_value;
        char    char_value;
        char*   string_value;

        struct node* parent;
        struct node* brother;
        struct node* child;
    };

    
}
%{
    //void showTreeRecursion(struct node currentNode);
    struct node* createNodeOnYYVal(struct node* newNode);
    struct node* createChildren(struct node* parent, struct node* child);
    struct node* createNode(int state);
    void showTree(struct node* root);
%}

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
    test    {struct node* root = createChildren(createNode(100), $1); showTree(root);};

//code {printf("endofcode\n");struct node* root = createChildren(createRootNode(), $1); showTree(root);}
//| %empty;

test:
    TK_IDENTIFICADOR TK_IDENTIFICADOR ';'
    
     {
       $$ = createNode(101); 
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

void showTreeRecursion(struct node* currentNode, int treeLevel);

void yyerror (char const *s){
    if(yytext == NULL || yytext[0] == '\0')
        printf("Line %d: %s in last token of line\n",yylineno, s);
    else
        printf("Line %d: %s near \"%s\"\n",yylineno, s, yytext);
}

struct node* createChildren(struct node* parent, struct node* child){
     struct node* nodeIterator;
     
     printf(" create children ");
     printf ("To be inserted: %s \n", child->token_value);
     if(parent->child != NULL){
         nodeIterator = parent->child;
         printf(" %s ", nodeIterator->token_value);
         while(nodeIterator->brother != NULL){
             
             nodeIterator = nodeIterator->brother;
             printf(" %s ", nodeIterator->token_value);
         }
         nodeIterator->brother = child;
         printf(" \n ");
     }
     else{
         struct node* newNode = malloc(sizeof(struct node));
         newNode = child;
         parent->child = newNode;
    }
    return parent;
}

struct node* createNode(int state){
    struct node* newNode = malloc(sizeof(struct node));
    newNode->brother = NULL;
	newNode->parent = NULL;
	newNode->child = NULL;
    newNode->token_type   = state;  
    return newNode;
}
void showTree(struct node* root)
{
	printf("$\n");
    showTreeRecursion(root, 0);
}
void showTreeRecursion(struct node* currentNode, int treeLevel)
{
    for(int i = 0; i<treeLevel;i++)
        printf("\t"); 
    printf("OI");
    if(currentNode->child == NULL)
        printf("%s: %d\n",currentNode->token_value, currentNode->line_number);
    else{
        printf("%d\n",currentNode->token_type);
    }
    if (currentNode->child != NULL)
        showTreeRecursion(currentNode->child, treeLevel + 1);

    if (currentNode->brother != NULL)
        showTreeRecursion(currentNode->brother, treeLevel); 
}

void descompila (void *arvore){
    printf("Function descompila not yet implemented\n");
}
void libera (void *arvore){
    printf("Function libera not yet implemented\n");
}