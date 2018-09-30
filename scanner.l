/*INF UFRGS 2018/2// COMPILADORES SCHNORR // GRUPO KAPPA // GUILHERME CATTANI DE CASTRO 00243589 && CAIO F RODRIGUES 00261578*/
%x multilinecomment

%{
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

    #include "parser.tab.h"
%}

%%
int                                             {   struct node *newNode = malloc(sizeof(struct node));
                                                    newNode->line_number = yylineno;
                                                    newNode->token_type  = RESERVED_WORD;
                                                    newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_INT;
                                                };
float                                           {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_FLOAT;
                                                };
bool                                            {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_BOOL;
                                                };
char                                            {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_CHAR;
                                                };
string                                          {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_STRING;
                                                };
if                                              {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_IF;
                                                };
then                                            {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_THEN;
                                                };
else                                            {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_ELSE;
                                                };
while                                           {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_WHILE;
                                                };
do                                              {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_DO;
                                                };
input                                           {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_INPUT;
                                                };
output                                          {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_OUTPUT;
                                                };
return                                          {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_RETURN;
                                                };
const                                           {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_CONST;
                                                };
static                                          {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_STATIC;
                                                };
foreach                                         {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_FOREACH;
                                                };
for                                             {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_FOR;
                                                };
switch                                          {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_SWITCH;
                                                };
case                                            {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_CASE;
                                                };
break                                           {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_BREAK;
                                                };
continue                                        {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_CONTINUE;
                                                };
class                                           {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_CLASS;
                                                };
private                                         {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_PRIVATE;
                                                };
public                                          {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_PUBLIC;
                                                };
protected                                       {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = RESERVED_WORD;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_PR_PROTECTED;
                                                };




\<\=                                            {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = COMPOUND_OPERATOR;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_OC_LE;
                                                };   
\>\=                                            {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = COMPOUND_OPERATOR;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_OC_GE;
                                                };   
\=\=                                            {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = COMPOUND_OPERATOR;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_OC_EQ;
                                                };   
\!\=                                            {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = COMPOUND_OPERATOR;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_OC_NE;
                                                }; 
\&\&                                            {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = COMPOUND_OPERATOR;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_OC_AND;
                                                };   
\|\|                                            {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = COMPOUND_OPERATOR;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_OC_OR;
                                                };   
\>\>                                            {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = COMPOUND_OPERATOR;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_OC_SR;
                                                };   
\<\<                                            {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = COMPOUND_OPERATOR;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_OC_SL;
                                                };  
\%\>\%                                          {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = COMPOUND_OPERATOR;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_OC_FORWARD_PIPE;
                                                };
\%\|\%                                          {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = COMPOUND_OPERATOR;
													newNode->token_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_OC_BASH_PIPE;
                                                };




\,|\;|\:|\)|\(|\]|\[|\}|\{|\+|\-|\||\?          {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
                                                    ;
													newNode->token_type  = SPECIAL_CHARACTER;
													newNode->token_value = &yytext[0];
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    return yytext[0];
                                                }
\*|\/|\>|\<|\=|\!|\&|\%|\#|\^|\.|\$             {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = SPECIAL_CHARACTER;
													newNode->token_value = &yytext[0];
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;

                                                    return yytext[0];
                                                }




[0-9]*                                          {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type = LITERAL_INT;
													newNode->int_value = atoi(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_LIT_INT;
                                                }
[0-9]*\.([0-9]*((e|E)(\+|\-)?[0-9]*)?)?         {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = LITERAL_FLOAT;
													newNode->float_value = atof(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_LIT_FLOAT;
                                                }
false                                           {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = LITERAL_BOOL;
													newNode->bool_value  = FALSE;
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_LIT_FALSE;
                                                }
true                                            {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = LITERAL_BOOL;
													newNode->bool_value  = TRUE;
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_LIT_TRUE;
                                                }
\'([^\'\\\n]|\\n|\\\'|\\\\)\'                   {   struct node *newNode = malloc(sizeof(struct node));
													newNode->line_number = yylineno;
													newNode->token_type  = LITERAL_CHAR;
													newNode->char_value  = yytext[0];
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_LIT_CHAR;
                                                }
\"(\\\"|[^\"|\r\n|\r|\n])*\"                    {   struct node *newNode  = malloc(sizeof(struct node));
                                                    newNode->line_number  = yylineno;
                                                    newNode->token_type   = LITERAL_STRING;
                                                    newNode->string_value = strdup(yytext);
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;
                                                    
                                                    return TK_LIT_STRING;
                                                }

[A-Z_a-z][A-Z_a-z0-9]*                          {   struct node *newNode = malloc(sizeof(struct node));
                                                    newNode->line_number = yylineno;
                                                    newNode->token_type  = IDENTIFIER;
                                                    newNode->token_value = strdup(yytext);
                                                    
                                                    newNode->brother = NULL;
													newNode->parent = NULL;
													newNode->child = NULL;
													yylval.nodo = newNode;

                                                    return TK_IDENTIFICADOR;
                                                }
    
\/{2}.*                                         /* single line comment */
"/*"		                                    BEGIN(multilinecomment);
<multilinecomment>[^*\n]*	                    /* ignore everything that isn't newline or '*' */
<multilinecomment>"*"+[^*/\n]*                  /* ignore everything that isn't '*' followed by '/' */
<multilinecomment>\n		                    yylineno++;
<multilinecomment>"*"+"/"	                    BEGIN(INITIAL); /* source: flex man */
                     
\r\n                                            yylineno++;
\n                                              yylineno++;
\r                                              yylineno++;

" "                                             {};
.                                               return TOKEN_ERRO;
%%
int yywrap(){}

int get_line_number(void){
    return yylineno;
}
