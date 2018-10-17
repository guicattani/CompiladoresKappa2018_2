#include "auxiliar.h"


union value createValue(int type, char* text){
    union value value;
    switch (type){
        case RESERVED_WORD: //Reserved word
        case SPECIAL_CHARACTER: //Special Char 
        case COMPOUND_OPERATOR: //Compound Operator
        case NATUREZA_IDENTIFICADOR: //Identifier
            value.string_value = _strdup(text);
            break;
        case NATUREZA_LITERAL_INT: //Lit int
            value.int_value = atoi(text);
            break;
        case NATUREZA_LITERAL_BOOL: //Literal Bool
            if (!strcmp(text, "true"))
                value.bool_value = 1;
            else if (!strcmp(text, "false"))
                value.bool_value = 0;
            break;
        case NATUREZA_LITERAL_FLOAT: //Literal Float
            value.float_value = atof(text);
            break;
        case NATUREZA_LITERAL_CHAR: //Literal Char
        case NATUREZA_LITERAL_STRING: //Literal String
            value.string_value = _strdup(text);
            break;

        
    }
    return value;
}

int getSizeFromType(int type, int vectorSize, char* name){
    int size;
    switch(type){
        case NATUREZA_IDENTIFICADOR: //Identifier
            break;
        case NATUREZA_LITERAL_INT: //Lit int
            size = 4 * vectorSize;
            break;
        case NATUREZA_LITERAL_BOOL: //Literal Bool
            size = 1 * vectorSize;
            break;
        case NATUREZA_LITERAL_FLOAT: //Literal Float
            size = 8 * vectorSize;
            break;
        case NATUREZA_LITERAL_CHAR: //Literal Char
            size = 1 * vectorSize;
            break;
        case NATUREZA_LITERAL_STRING: //Literal String
            size = 1 * strlen(name) * vectorSize;
            break;



    }
    return size;
}
