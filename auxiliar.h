#ifndef AUXH
#define AUXH

#include <string.h>
#include <stdlib.h>

#define FALSE                       0
#define TRUE                        1

#define NATUREZA_LITERAL_INT        1
#define NATUREZA_LITERAL_FLOAT      2
#define NATUREZA_LITERAL_CHAR       3
#define NATUREZA_LITERAL_STRING     4
#define NATUREZA_LITERAL_BOOL       5
#define NATUREZA_IDENTIFICADOR      6
#define RESERVED_WORD               7
#define SPECIAL_CHARACTER           8
#define COMPOUND_OPERATOR           9
#define NATUREZA_FUNC               10 //Natureza Vetor SÓ é utilizado na declaração de funções
#define NATUREZA_CLASSE             11 //Natureza classe SÓ é utilizado na declaração de classes
#define NATUREZA_VETOR              12 //Natureza vetor dá override nas outras naturezas, cheque o TIPO dele para saber do que ele é vetor
#define NATUREZA_VETOR_CLASSE       13 //Vetor que também é uma classe hu3

#define AST_PROGRAMA            "Estado Programa"
#define AST_LITERAL             "Estado literal"
#define AST_PIPE                "Estado pipe"
#define AST_USERVARORLIT        "Estado userVariableOrLiteral"
#define AST_TYPE                "Estado type"
#define AST_PRIMITIVETYPE       "Estado primitiveType"
#define AST_SHIFT               "Estado shift"
#define AST_ACCESSMOD           "Estado accessModifiers"
#define AST_CONSTMOD            "Estado constModifier"
#define AST_VECTORMOD           "Estado vectorModifier"
#define AST_VECTORLIST          "Estado vectorList"
#define AST_CODE                "Estado code"
#define AST_DECLARATION         "Estado declaration"
#define AST_CLASSDEC            "Estado classDeclaration"
#define AST_FIELDS              "Estado fields"
#define AST_GLOBALVARDEC        "Estado globalVarDeclaration"
#define AST_FUNCDEC             "Estado functionDeclaration"
#define AST_FUNCHEAD            "Estado functionHead"
#define AST_FUNCCOMMANDSBLOCK   "Estado functionCommandsBlock"
#define AST_COMMANDSBLOCK       "Estado commandsBlock"
#define AST_COMMANDSLIST        "Estado commandsList"
#define AST_FUNCARGLIST         "Estado functionArgumentsList"
#define AST_FUNCARGELEM         "Estado functionArgumentElements"
#define AST_COMMAND             "Estado command"
#define AST_COMMANDSIMPLE       "Estado commandSimple"
#define AST_LOCALVARDEC         "Estado localVarDeclaration"
#define AST_LOCALVARCDEC        "Estado localVarCompleteDeclaration"
#define AST_LOCALVARINIT        "Estado localVarInit"
#define AST_ATTRIBUTION         "Estado attribution"
#define AST_PRIMATTR            "Estado primitiveAttribution"
#define AST_USERTYPEATTR        "Estado userTypeAttribution"
#define AST_EXPRESSION          "Estado expression"
#define AST_SIMPLEEXP           "Estado simpleExpression"
#define AST_LOWPTFREXP          "Estado lowPrecedenceTwoFoldRecursiveExpression"
#define AST_MEDPTFREXP          "Estado mediumPrecedenceTwoFoldRecursiveExpression"
#define AST_HIGHPTFREXP         "Estado highPrecedenceTwoFoldRecursiveExpression"
#define AST_ONEFREXP            "Estado oneFoldRecursiveExpression"
#define AST_OPERATOR            "Estado operator"
#define AST_UNARYOPERATOR       "Estado unaryOperator"
#define AST_UNARYSIMBOL         "Estado unarySimbol"
#define AST_ARITHMETICOP        "Estado arithmeticOperator"
#define AST_PRECARITHOP         "Estado precedentArithmeticOperator"
#define AST_COMPARISONOP        "Estado comparisonOperator"
#define AST_EXPLIST             "Estado expressionList"
#define AST_FUNCTIONCALL        "Estado functionCall"
#define AST_FUNCCALLARGS        "Estado functionCallArguments"
#define AST_FUNCCALLARGSL       "Estado functionCallArgumentsList"
#define AST_FUNCCALLARG         "Estado functionCallArgument"
#define AST_SHIFTCOMMAND        "Estado shiftCommand"
#define AST_FLUXCONTCMD         "Estado fluxControlCommand"
#define AST_CONDFLUXCONT        "Estado conditionalFluxControl"
#define AST_ITERFLUXCONT        "Estado iterativeFluxControl"
#define AST_SELECFLUXCONT       "Estado selectionFluxControl"
#define AST_COMMANDSIMPLELIST   "Estado commandSimpleList"
#define AST_PIPECOMMANDS        "Estado pipeCommands"

/*  Error codes from task specification */
/* Verificação de declarações */
#define ERR_UNDECLARED          10 //identificador não declarado
#define ERR_DECLARED            11 //identificador já declarado

/* Uso correto de identificadores */
#define ERR_VARIABLE            20 //identificador deve ser utilizado como variável
#define ERR_VECTOR              21 //identificador deve ser utilizado como vetor
#define ERR_FUNCTION            22 //identificador deve ser utilizado como função
#define ERR_USER                23 //identificador deve ser utilizado como de usuário
#define ERR_CLASS               24 //identificador deve ser utilizado como variável normal e não como classe
#define ERR_CLASS_ID_NOT_FOUND  25 //Classe existe, porém o campo desejado não
/* Tipos e tamanho de dados */
#define ERR_WRONG_TYPE          30 //tipos incompatíveis
#define ERR_STRING_TO_X         31 //coerção impossível de var do tipo string
#define ERR_CHAR_TO_X           32 //coerção impossível de var do tipo char
#define ERR_USER_TO_X           33 //coerção impossível de var do tipo de usuário

/* Argumentos e parâmetros */
#define ERR_MISSING_ARGS        40 //faltam argumentos 
#define ERR_EXCESS_ARGS         41 //sobram argumentos 
#define ERR_WRONG_TYPE_ARGS     42 //argumentos incompatíveis

/* Verificação de tipos em comandos */
#define ERR_WRONG_PAR_INPUT     50 //parâmetro não é identificador
#define ERR_WRONG_PAR_OUTPUT    51 //parâmetro não é literal string ou expressão
#define ERR_WRONG_PAR_RETURN    52 //parâmetro não é expressão compatível com tipo do retorno

/* Verificação de tipos existente */
#define ERR_TYPE_UNDECLARED     60  //Tipo não declarado


union value{
        int     int_value;
        int     bool_value;
        float   float_value;
        char    char_value;
        char*   string_value;
    };

union value createValue(int type, char* text);

//Given a token_value, gives back a union Value with the value in the right position
int getSizeFromType(int type, int vectorSize, int sizeOfString);
int parseType(char* typeString);


#endif