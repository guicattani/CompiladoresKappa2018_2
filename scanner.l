/*INF UFRGS 2018/2// COMPILADORES SCHNORR // GRUPO KAPPA // GUILHERME CATTANI DE CASTRO 00243589 && CAIO F RODRIGUES 00261578*/
%x multilinecomment

%{
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

    #include "parser.tab.h"
    #include "ASTree.h"
%}

%%
int                                             {   
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_INT;
                                                };
float                                           {   
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_FLOAT;
                                                };
bool                                            { 
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_BOOL;
                                                };
char                                            {   
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_CHAR;
                                                };
string                                          {   
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_STRING;
                                                };
if                                              {   
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_IF;
                                                };
then                                            {   
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_THEN;
                                                };
else                                            {   
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_ELSE;
                                                };
while                                           {   
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_WHILE;
                                                };
do                                              {  
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_DO;
                                                };
input                                           {   
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_INPUT;
                                                };
output                                          {   
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_OUTPUT;
                                                };
return                                          {   
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_RETURN;
                                                };
const                                           {   
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_CONST;
                                                };
static                                          {   
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_STATIC;
                                                };
foreach                                         {   
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_FOREACH;
                                                };
for                                             {   
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_FOR;
                                                };
switch                                          {   
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_SWITCH;
                                                };
case                                            {   
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_CASE;
                                                };
break                                           {  
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_BREAK;
                                                };
continue                                        {   
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_CONTINUE;
                                                };
class                                           {   
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_CLASS;
                                                };
private                                         {   
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_PRIVATE;
                                                };
public                                          {   
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_PUBLIC;
                                                };
protected                                       {   
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_PROTECTED;
                                                };




\<\=                                            {   
                                                    yylval.nodo = createLeaf(yylineno, COMPOUND_OPERATOR, yytext);
                                                    return TK_OC_LE;
                                                };   
\>\=                                            {   
                                                    yylval.nodo = createLeaf(yylineno, COMPOUND_OPERATOR, yytext);
                                                    return TK_OC_GE;
                                                };   
\=\=                                            {  
                                                    yylval.nodo = createLeaf(yylineno, COMPOUND_OPERATOR, yytext);
                                                    return TK_OC_EQ;
                                                };   
\!\=                                            {   
                                                    yylval.nodo = createLeaf(yylineno, COMPOUND_OPERATOR, yytext);
                                                    return TK_OC_NE;
                                                }; 
\&\&                                            {   
                                                    yylval.nodo = createLeaf(yylineno, COMPOUND_OPERATOR, yytext);
                                                    return TK_OC_AND;
                                                };   
\|\|                                            {   
                                                    yylval.nodo = createLeaf(yylineno, COMPOUND_OPERATOR, yytext);
                                                    return TK_OC_OR;
                                                };   
\>\>                                            {   
                                                    yylval.nodo = createLeaf(yylineno, COMPOUND_OPERATOR, yytext);
                                                    return TK_OC_SR;
                                                };   
\<\<                                            {   
                                                    yylval.nodo = createLeaf(yylineno, COMPOUND_OPERATOR, yytext);
                                                    return TK_OC_SL;
                                                };  
\%\>\%                                          {   
                                                    yylval.nodo = createLeaf(yylineno, COMPOUND_OPERATOR, yytext);
                                                    return TK_OC_FORWARD_PIPE;
                                                };
\%\|\%                                          {   
                                                    yylval.nodo = createLeaf(yylineno, COMPOUND_OPERATOR, yytext);
                                                    return TK_OC_BASH_PIPE;
                                                };




\,|\;|\:|\)|\(|\]|\[|\}|\{|\+|\-|\||\?          {  
                                                    yylval.nodo = createLeaf(yylineno, SPECIAL_CHARACTER, yytext);
                                                    return yytext[0];
                                                }
\*|\/|\>|\<|\=|\!|\&|\%|\#|\^|\.|\$             {   
                                                    yylval.nodo = createLeaf(yylineno, SPECIAL_CHARACTER, yytext);
                                                    return yytext[0];
                                                }




[0-9]*                                          {   
                                                    yylval.nodo = createLeaf(yylineno, LITERAL_INT, yytext);
                                                    return TK_LIT_INT;
                                                }
[0-9]*\.([0-9]*((e|E)(\+|\-)?[0-9]*)?)?         {   
                                                    yylval.nodo = createLeaf(yylineno, LITERAL_FLOAT, yytext);
                                                    return TK_LIT_FLOAT;
                                                }
false                                           {   
                                                    yylval.nodo = createLeaf(yylineno, LITERAL_BOOL, yytext);
                                                    return TK_LIT_FALSE;
                                                }
true                                            {   
                                                    yylval.nodo = createLeaf(yylineno, LITERAL_BOOL, yytext);   
                                                    return TK_LIT_TRUE;
                                                }
\'([^\'\\\n]|\\n|\\\'|\\\\)\'                   {   
                                                    yylval.nodo = createLeaf(yylineno, LITERAL_CHAR, yytext);
                                                    return TK_LIT_CHAR;
                                                }
\"(\\\"|[^\"|\r\n|\r|\n])*\"                    {  
                                                    yylval.nodo = createLeaf(yylineno, LITERAL_STRING, yytext);
                                                    return TK_LIT_STRING;
                                                }

[A-Z_a-z][A-Z_a-z0-9]*                          {   
                                                    yylval.nodo = createLeaf(yylineno, IDENTIFIER, yytext);
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

\t                                              {};
" "                                             {};
.                                               return TOKEN_ERRO;
%%
int yywrap(){}

int get_line_number(void){
    return yylineno;
}
