#include "auxiliar.h"


union value createValue(int type, char* text){
    union value value;
    switch (type){
        case RESERVED_WORD: //Reserved word
        case SPECIAL_CHARACTER: //Special Char 
        case COMPOUND_OPERATOR: //Compound Operator
        case NATUREZA_IDENTIFICADOR: //Identifier
            value.string_value = strdup(text);
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
            value.string_value = strdup(text);
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

int calculateImplicitConvert(int variableType, int expressionType){
    if(variableType == expressionType)
        return variableType;
    
    if(variableType == NATUREZA_LITERAL_STRING || variableType == NATUREZA_LITERAL_CHAR ||
       expressionType == NATUREZA_LITERAL_STRING || expressionType == NATUREZA_LITERAL_CHAR)
       return -1; //ERROR Can't implicit convert string or char
    
    if(variableType == NATUREZA_LITERAL_FLOAT && expressionType == NATUREZA_LITERAL_INT)
        return NATUREZA_LITERAL_FLOAT;
    if(variableType == NATUREZA_LITERAL_FLOAT && expressionType == NATUREZA_LITERAL_BOOL)
        return NATUREZA_LITERAL_FLOAT;
    
    if(variableType == NATUREZA_LITERAL_BOOL && expressionType == NATUREZA_LITERAL_INT)
        return NATUREZA_LITERAL_BOOL;
    if(variableType == NATUREZA_LITERAL_BOOL && expressionType == NATUREZA_LITERAL_FLOAT)
        return NATUREZA_LITERAL_BOOL;

    if(variableType == NATUREZA_LITERAL_INT && expressionType == NATUREZA_LITERAL_BOOL)
        return NATUREZA_LITERAL_INT;
    if(variableType == NATUREZA_LITERAL_INT && expressionType == NATUREZA_LITERAL_FLOAT)
        return NATUREZA_LITERAL_INT;
    
    return -1; //ERROR can't convert
}


int calculateTypeInfer(struct node* node){
    if(node == NULL)
        return 0;

    int childInfer = calculateTypeInfer(node->child);
    int brotherInfer = calculateTypeInfer(node->brother);

    int nodeType = node->token_type;

    if(childInfer != 0)
        nodeType = childInfer;

    if(brotherInfer == NATUREZA_LITERAL_INT &&
       nodeType == NATUREZA_LITERAL_INT)
       return NATUREZA_LITERAL_INT;
       
    if(brotherInfer == NATUREZA_LITERAL_FLOAT &&
       nodeType == NATUREZA_LITERAL_FLOAT)
       return NATUREZA_LITERAL_FLOAT;
       
    if(brotherInfer == NATUREZA_LITERAL_BOOL &&
       nodeType == NATUREZA_LITERAL_BOOL)
       return NATUREZA_LITERAL_BOOL;
       
    if(brotherInfer == NATUREZA_LITERAL_FLOAT &&
       nodeType == NATUREZA_LITERAL_INT ||
       brotherInfer == NATUREZA_LITERAL_INT &&
       nodeType == NATUREZA_LITERAL_FLOAT )
       return NATUREZA_LITERAL_FLOAT;
       
    if(brotherInfer == NATUREZA_LITERAL_BOOL &&
       nodeType == NATUREZA_LITERAL_INT ||
       brotherInfer == NATUREZA_LITERAL_INT &&
       nodeType == NATUREZA_LITERAL_BOOL)
       return NATUREZA_LITERAL_INT;

    if(brotherInfer == NATUREZA_LITERAL_BOOL &&
       nodeType == NATUREZA_LITERAL_FLOAT ||
       brotherInfer == NATUREZA_LITERAL_FLOAT &&
       nodeType == NATUREZA_LITERAL_BOOL)
       return NATUREZA_LITERAL_FLOAT;
}
