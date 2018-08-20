%{
#include "tokens.h"
%}

%%
int         return TK_PR_INT;
float       return TK_PR_FLOAT;
bool        return TK_PR_BOOL;
char        return TK_PR_CHAR;
if          return TK_PR_IF;
then        return TK_PR_THEN;
else        return TK_PR_ELSE;
while       return TK_PR_WHILE;
do          return TK_PR_DO;
input       return TK_PR_INPUT;
ouput       return TK_PR_OUTPUT;
return      return TK_PR_RETURN;
const       return TK_PR_CONST;
static      return TK_PR_STATIC;
foreach     return TK_PR_FOREACH;
for         return TK_PR_FOR;
switch      return TK_PR_SWITCH;
case        return TK_PR_CASE;
break       return TK_PR_BREAK;
continue    return TK_PR_CONTINUE;
class       return TK_PR_CLASS;
private     return TK_PR_PRIVATE;
public      return TK_PR_PUBLIC;
protected   return TK_PR_PROTECTED;
,           return ',';
;           return ';';
:           return ':';
\)          return ')';
\(          return '(';
\]          return ']';
\[          return '[';
\}          return '}';
\{          return '{';
\+          return '+';
-           return '-';
\|          return '|';
\?          return '?';
\*          return '*';
\/          return '/';
\>          return '>';
\<          return '<';
=           return '=';
!           return '!';
&           return '&';
%           return '%';
#           return '#';
\^          return '^';
\.          return '.';
\$          return '$';
%%

/* int float bool char string if then else
while do input output return const static
foreach for switch case break continue class
private public protected */

//TODO
/* Compostos
<=    >=    ==     !=    &&    ||    >>     <<     %>%    %|%
*/

/*Identificadores*/

/*Literais*/

//TODO logic
int current_line = 0;

int get_line_number(void){
    return current_line;
}
