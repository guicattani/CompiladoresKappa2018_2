/*INF UFRGS 2018/2// COMPILADORES SCHNORR // GRUPO KAPPA // GUILHERME CATTANI DE CASTRO 00243589 && CAIO F RODRIGUES 00261578*/
%x multilinecomment

%{
#define FALSE             0
#define TRUE              1

#define RESERVED_WORD     2
#define SPECIAL_CHARACTER 3
#define COMPOUND_OPERATOR 4
#define IDENTIFIER        5
#define LITERAL           6

#include "parser.tab.h"
%}

%%
int                                             {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_INT;
                                                }
float                                           {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_FLOAT;
                                                };
bool                                            {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_BOOL;
                                                };
char                                            {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_CHAR;
                                                };
string                                          {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_STRING;
                                                };
if                                              {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_IF;
                                                };
then                                            {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_THEN;
                                                };
else                                            {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_ELSE;
                                                };
while                                           {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_WHILE;
                                                };
do                                              {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_DO;
                                                };
input                                           {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_INPUT;
                                                };
output                                          {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_OUTPUT;
                                                };
return                                          {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_RETURN;
                                                };
const                                           {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_CONST;
                                                };
static                                          {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_STATIC;
                                                };
foreach                                         {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_FOREACH;
                                                };
for                                             {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_FOR;
                                                };
switch                                          {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_SWITCH;
                                                };
case                                            {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_CASE;
                                                };
break                                           {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_BREAK;
                                                };
continue                                        {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_CONTINUE;
                                                };
class                                           {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_CLASS;
                                                };
private                                         {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_PRIVATE;
                                                };
public                                          {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_PUBLIC;
                                                };
protected                                       {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = RESERVED_WORD;
                                                    return TK_PR_PROTECTED;
                                                };




\<\=                                            {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = COMPOUND_OPERATOR;
                                                    return TK_OC_LE;
                                                };   
\>\=                                            {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = COMPOUND_OPERATOR;
                                                    return TK_OC_GE;
                                                };   
\=\=                                            {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = COMPOUND_OPERATOR;
                                                    return TK_OC_EQ;
                                                };   
\!\=                                            {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = COMPOUND_OPERATOR;
                                                    return TK_OC_NE;
                                                }; 
\&\&                                            {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = COMPOUND_OPERATOR;
                                                    return TK_OC_AND;
                                                };   
\|\|                                            {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = COMPOUND_OPERATOR;
                                                    return TK_OC_OR;
                                                };   
\>\>                                            {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = COMPOUND_OPERATOR;
                                                    return TK_OC_SR;
                                                };   
\<\<                                            {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = COMPOUND_OPERATOR;
                                                    return TK_OC_SL;
                                                };  
\%\>\%                                          {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = COMPOUND_OPERATOR;
                                                    return TK_OC_FORWARD_PIPE;
                                                };
\%\|\%                                          {   yylval.line_number = yylineno;
                                                    yylval.token_value = strdup(yytext);
                                                    yylval.token_type = COMPOUND_OPERATOR;
                                                    return TK_OC_FORWARD_PIPE;
                                                };




\,|\;|\:|\)|\(|\]|\[|\}|\{|\+|\-|\||\?          {   yylval.line_number = yylineno;
                                                    yylval.token_value = &yytext[0];
                                                    yylval.token_type = SPECIAL_CHARACTER;
                                                }
\*|\/|\>|\<|\=|\!|\&|\%|\#|\^|\.|\$             {   yylval.line_number = yylineno;
                                                    yylval.token_value = &yytext[0];
                                                    yylval.token_type = SPECIAL_CHARACTER;
                                                }




[0-9]*                                          {   yylval.line_number = yylineno;
                                                    yylval.token_type = LITERAL;
                                                    yylval.int_value = atoi(yytext);
                                                    return TK_LIT_INT;
                                                }
[0-9]*\.([0-9]*((e|E)(\+|\-)?[0-9]*)?)?         {   yylval.line_number = yylineno;
                                                    yylval.token_type = LITERAL;
                                                    yylval.float_value = atof(yytext);
                                                    return TK_LIT_FLOAT;
                                                }
false                                           {   yylval.line_number = yylineno;
                                                    yylval.token_type = LITERAL;
                                                    yylval.bool_value = FALSE;
                                                    return TK_LIT_FALSE;
                                                }
true                                            {   yylval.line_number = yylineno;
                                                    yylval.token_type = LITERAL;
                                                    yylval.bool_value = TRUE;
                                                    return TK_LIT_TRUE;
                                                }
\'([^\'\\\n]|\\n|\\\'|\\\\)\'                   {   yylval.line_number = yylineno;
                                                    yylval.token_type = LITERAL;
                                                    yylval.char_value = yytext[0];
                                                    return TK_LIT_CHAR;
                                                }
\"(\\\"|[^\"|\r\n|\r|\n])*\"                    {   yylval.line_number = yylineno;
                                                    yylval.token_type = LITERAL;
                                                    yylval.string_value = strdup(yytext);
                                                    return TK_LIT_STRING;
                                                }

[A-Z_a-z][A-Z_a-z0-9]*                          return TK_IDENTIFICADOR;
    
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
