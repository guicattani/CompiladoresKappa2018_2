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
        {
            if(value.string_value != NULL)
                free(value.string_value);
            value.string_value = strdup(text);
            break;
        }

        
    }
    return value;
}

int getSizeFromType(int type, int vectorSize, int sizeOfString){
    int size = 0;
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
            size = -1;
            break;



    }
    return size;
}

//return conversion if possible, -1 if it isn't
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
        return NATUREZA_LITERAL_INT;
    if(variableType == NATUREZA_LITERAL_BOOL && expressionType == NATUREZA_LITERAL_FLOAT)
        return NATUREZA_LITERAL_FLOAT;

    if(variableType == NATUREZA_LITERAL_INT && expressionType == NATUREZA_LITERAL_BOOL)
        return NATUREZA_LITERAL_INT;
    if(variableType == NATUREZA_LITERAL_INT && expressionType == NATUREZA_LITERAL_FLOAT)
        return NATUREZA_LITERAL_FLOAT;
    
    return -1; //ERROR can't convert
}

int parseType(char* typeString){
    if(strcmp(typeString, "int") == 0)
        return NATUREZA_LITERAL_INT;
    if(strcmp(typeString, "float") == 0)
        return NATUREZA_LITERAL_FLOAT;
    if(strcmp(typeString, "bool") == 0)
        return NATUREZA_LITERAL_BOOL;
    if(strcmp(typeString, "char") == 0)
        return NATUREZA_LITERAL_CHAR;
    if(strcmp(typeString, "string") == 0)
        return NATUREZA_LITERAL_STRING;
    else
        return NATUREZA_IDENTIFICADOR;
}


struct functionLabels* addFunctionLabelList(struct functionLabels* list, char* name, char* label){
    struct functionLabels* node = NULL;
    node = malloc(sizeof(struct functionLabels));
    node->label = strdup(label);
    node->functionName = strdup(name);
    node->next = NULL;
 
    if(list == NULL){
        return node;
    }
    else{
        struct functionLabels* iter = list;
        while(iter->next != NULL){
            iter = iter->next;
        }
        iter->next = node;
    }
    return list;
}

//Given a name, finds the label of that function in a functionLabel structure
//Returns NULL if not found
char* findLabel(struct functionLabels* list, char* name){
    
    while(list != NULL){
        if(strcmp(name, list->functionName) == 0){
            return list->label;
        }    
        list = list->next;
    }
    return NULL;
}

void freeLabels(struct functionLabels* list){
    while (list != NULL){
        if(list->label != NULL)
            free(list->label);
        if(list->functionName != NULL)
            free(list->functionName);
        struct functionLabels* temp = list;
        list = list->next;
        free(temp); 
    }
}
