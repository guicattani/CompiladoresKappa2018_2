%x multilinecomment
%{
#include "tokens.h"
int current_line = 1;
%}

%%
int                                             return TK_PR_INT;
float                                           return TK_PR_FLOAT;
bool                                            return TK_PR_BOOL;
char                                            return TK_PR_CHAR;
string                                          return TK_PR_STRING;
if                                              return TK_PR_IF;
then                                            return TK_PR_THEN;
else                                            return TK_PR_ELSE;
while                                           return TK_PR_WHILE;
do                                              return TK_PR_DO;
input                                           return TK_PR_INPUT;
ouput                                           return TK_PR_OUTPUT;
return                                          return TK_PR_RETURN;
const                                           return TK_PR_CONST;
static                                          return TK_PR_STATIC;
foreach                                         return TK_PR_FOREACH;
for                                             return TK_PR_FOR;
switch                                          return TK_PR_SWITCH;
case                                            return TK_PR_CASE;
break                                           return TK_PR_BREAK;
continue                                        return TK_PR_CONTINUE;
class                                           return TK_PR_CLASS;
private                                         return TK_PR_PRIVATE;
public                                          return TK_PR_PUBLIC;
protected                                       return TK_PR_PROTECTED;
\<\=                                            return TK_OC_LE;   
\>\=                                            return TK_OC_GE;   
\=\=                                            return TK_OC_EQ;    
\!\=                                            return TK_OC_NE; 
\&\&                                            return TK_OC_AND;   
\|\|                                            return TK_OC_OR;   
\>\>                                            return TK_OC_SL;    
\<\<                                            return TK_OC_SR;    
\%\>\%                                          return TK_OC_FORWARD_PIPE;
\%\|\%                                          return TK_OC_BASH_PIPE;

\,|\;|\:|\)|\(|\]|\[|\}|\{|\+|\-|\||\?          return yytext[0];
\*|\/|\>|\<|\=|\!|\&|\%|\#|\^|\.|\$             return yytext[0];

(\-|\+)?[0-9]*                                  return TK_LIT_INT;
(\-|\+)?[0-9]*\.([0-9]*((e|E)(\+|\-)?[0-9]*)?)? return TK_LIT_FLOAT;
false                                           return TK_LIT_FALSE;
true                                            return TK_LIT_TRUE;
\'([^\'\\\n]|\\n|\\\'|\\\\)\'                   return TK_LIT_CHAR;
\"(\\\"|[^\"|\r\n|\r|\n])*\"                    return TK_LIT_STRING;

[A-Z_a-z][A-Z_a-z0-9]*                          return TK_IDENTIFICADOR;
    
\/{2}.*                                         /* single line comment */
"/*"		                                    BEGIN(multilinecomment);
<multilinecomment>[^*\n]*	                    /* ignore everything that isn't newline or '*' */
<multilinecomment>"*"+[^*/\n]*                  /* ignore everything that isn't '*' followed by '/' */
<multilinecomment>\n		                    ++current_line;
<multilinecomment>"*"+"/"	                    BEGIN(INITIAL); /* source: flex man */
                     
\r\n                                            current_line++;
\n                                              current_line++;
\r                                              current_line++;

.                                               return TOKEN_ERRO;
%%

int get_line_number(void){
    return current_line;
}
