/*INF UFRGS 2018/2// COMPILADORES SCHNORR // GRUPO KAPPA // GUILHERME CATTANI DE CASTRO 00243589 && CAIO F RODRIGUES 00261578*/
%x multilinecomment

%{
    #include "auxiliar.h"
    #include "parser.tab.h"
    #include "ASTree.h"
    int yycolno = 0;
    char* previous_text = NULL;
    void add_col();

%}

%%
int                                             {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_INT;
                                                };
float                                           {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_FLOAT;
                                                };
bool                                            { 
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_BOOL;
                                                };
char                                            {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_CHAR;
                                                };
string                                          {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_STRING;
                                                };
if                                              {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_IF;
                                                };
then                                            {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_THEN;
                                                };
else                                            {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_ELSE;
                                                };
while                                           {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_WHILE;
                                                };
do                                              {  
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_DO;
                                                };
input                                           {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_INPUT;
                                                };
output                                          {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_OUTPUT;
                                                };
return                                          {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_RETURN;
                                                };
const                                           {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_CONST;
                                                };
static                                          {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_STATIC;
                                                };
foreach                                         {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_FOREACH;
                                                };
for                                             {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_FOR;
                                                };
switch                                          {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_SWITCH;
                                                };
case                                            {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_CASE;
                                                };
break                                           {  
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_BREAK;
                                                };
continue                                        {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_CONTINUE;
                                                };
class                                           {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_CLASS;
                                                };
private                                         {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_PRIVATE;
                                                };
public                                          {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_PUBLIC;
                                                };
protected                                       {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, RESERVED_WORD, yytext);
                                                    return TK_PR_PROTECTED;
                                                };




\<\=                                            {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, COMPOUND_OPERATOR, yytext);
                                                    return TK_OC_LE;
                                                };   
\>\=                                            {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, COMPOUND_OPERATOR, yytext);
                                                    return TK_OC_GE;
                                                };   
\=\=                                            {  
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, COMPOUND_OPERATOR, yytext);
                                                    return TK_OC_EQ;
                                                };   
\!\=                                            {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, COMPOUND_OPERATOR, yytext);
                                                    return TK_OC_NE;
                                                }; 
\&\&                                            {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, COMPOUND_OPERATOR, yytext);
                                                    return TK_OC_AND;
                                                };   
\|\|                                            {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, COMPOUND_OPERATOR, yytext);
                                                    return TK_OC_OR;
                                                };   
\>\>                                            {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, COMPOUND_OPERATOR, yytext);
                                                    return TK_OC_SR;
                                                };   
\<\<                                            {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, COMPOUND_OPERATOR, yytext);
                                                    return TK_OC_SL;
                                                };  
\%\>\%                                          {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, COMPOUND_OPERATOR, yytext);
                                                    return TK_OC_FORWARD_PIPE;
                                                };
\%\|\%                                          {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, COMPOUND_OPERATOR, yytext);
                                                    return TK_OC_BASH_PIPE;
                                                };




\,|\;|\:|\)|\(|\]|\[|\}|\{|\+|\-|\||\?          {  
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, SPECIAL_CHARACTER, yytext);
                                                    return yytext[0];
                                                    
                                                }
\*|\/|\>|\<|\=|\!|\&|\%|\#|\^|\.|\$             {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, SPECIAL_CHARACTER, yytext);
                                                    return yytext[0];
                                                }




[0-9]*                                          {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, NATUREZA_LITERAL_INT, yytext);
                                                    return TK_LIT_INT;
                                                }
[0-9]*\.([0-9]*((e|E)(\+|\-)?[0-9]*)?)?         {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, NATUREZA_LITERAL_FLOAT, yytext);
                                                    return TK_LIT_FLOAT;
                                                }
false                                           {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, NATUREZA_LITERAL_BOOL, yytext);
                                                    return TK_LIT_FALSE;
                                                }
true                                            {   
                                                    yylval.nodo = createLeaf(yylineno, NATUREZA_LITERAL_BOOL, yytext);   
                                                    return TK_LIT_TRUE;
                                                }
\'([^\'\\\n]|\\n|\\\'|\\\\)\'                   {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, NATUREZA_LITERAL_CHAR, yytext);
                                                    return TK_LIT_CHAR;
                                                }
\"(\\\"|[^\"|\r\n|\r|\n])*\"                    {  
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, NATUREZA_LITERAL_STRING, yytext);
                                                    return TK_LIT_STRING;
                                                }

[A-Z_a-z][A-Z_a-z0-9]*                          {   
                                                    add_col();
                                                    yylval.nodo = createLeaf(yylineno, NATUREZA_IDENTIFICADOR, yytext);
                                                    return TK_IDENTIFICADOR;
                                                }
    
\/{2}.*                                         /* single line comment */
"/*"		                                    BEGIN(multilinecomment);
<multilinecomment>[^*\n]*	                    { add_col();}
<multilinecomment>"*"+[^*/\n]*                  { add_col();}
<multilinecomment>\n		                    {yylineno++; yycolno = 0;};
<multilinecomment>"*"+"/"	                    BEGIN(INITIAL); /* source: flex man */
                     
\r\n                                            {yylineno++; yycolno = 0;}
\n                                              {yylineno++; yycolno = 0;}
\r                                              {yylineno++; yycolno = 0;};

\t                                              { add_col();};
" "                                             {add_col();};
.                                               return TOKEN_ERRO;
%%
int yywrap(){
    yycolno = 0;
    free(previous_text);
    return 0;
}

int get_line_number(void){
    return yylineno;
}

void add_col(){
    if (previous_text != NULL){

        if(strcmp(previous_text,"\t") == 0)
            yycolno += 4;
        else    
            yycolno += strlen(previous_text);

        free(previous_text);
    }
    previous_text = strdup(yytext);
}