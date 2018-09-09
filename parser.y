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

%right '&' '*' '#'


%%

programa:
      code 
    | %empty;

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
      TK_LIT_FALSE 
    | TK_LIT_FLOAT 
    | TK_LIT_INT 
    | TK_LIT_TRUE;

number:
      TK_LIT_FLOAT 
    | TK_LIT_INT;

pipe:
      TK_OC_BASH_PIPE
    | TK_OC_FORWARD_PIPE;

idOrLiteral:
      id 
    | literal;

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
      TK_OC_SR 
    | TK_OC_SL;
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
    functionHead commandsBlock;

functionHead:
    staticModifier type id '(' functionArgumentsList ')'

commandsBlock:
    '{' commandsList '}';

commandsList:
      command commandsList
    | %empty;

functionArgumentsList:
      functionArgumentElements 
    | %empty;

functionArgumentElements:
      constModifier type id 
    | constModifier type id ',' functionArgumentElements;

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
      primitiveType id localVarInit //If it starts with ID and is initialized
    | userType id;  


localVarCompleteDeclaration:
      TK_PR_STATIC TK_PR_CONST localVarDeclaration 
    | TK_PR_CONST localVarDeclaration 
    | localVarDeclaration;


    
//Initialization of variable
localVarInit:
      TK_OC_LE idOrLiteral 
    | %empty;

attribution:
      primitiveAttribution
    | userTypeAttribution;

primitiveAttribution:
    id vectorModifier '=' expression ;

userTypeAttribution:
    id vectorModifier '$' classField '=' expression ;

expression:
      unaryOperator unifiedExpression;

unifiedExpression:
      comparableLiteral 
    | comparableLiteral operator '(' expression ')'
    | '(' expression ')'
    | '(' expression ')' operator expression
    | compositeExpression;

compositeExpression:
      comparableLiteral operator comparableLiteral operator expression
    | comparableLiteral operator comparableLiteral;

operator:
      arithmeticOperator
    | comparisonOperator;

unaryOperator:
      '+' 
    | '-'
    | '!'
    | '&'
    | '*'
    | '?'
    | '#'
    | %empty;

arithmeticOperator:
      '+' 
    | '-'
    | '*'
    | '/'
    | '%'
    | '|'
    | '&'
    | '^';

comparisonOperator:
      TK_OC_LE
    | TK_OC_GE
    | TK_OC_EQ
    | TK_OC_NE
    | TK_OC_AND
    | TK_OC_OR
    | '<'
    | '>';

//ternaryOperator:
//    expression '?' expression ':' expression;

expressionList:
      expression 
    | expression ',' expressionList

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
      TK_PR_FOREACH '(' id ':' expressionList ')' commandsBlock;
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
    printf("%d:ERROR: %s\n",yylineno, s);
}