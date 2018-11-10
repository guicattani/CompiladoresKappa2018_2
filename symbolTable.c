#include "symbolTable.h"
#include <stdio.h>

struct contextStack* contextStack = NULL;

//Create a new context for the currentContext by pushing a new symbol table
//Since it is a stack, the table has to be moved to the front of the stack
void createContext(){
    struct contextStack* top = malloc(sizeof(struct contextStack));
    top->currentContext = NULL;
    top->previousContext = contextStack;
    rfpOffset = 0;
    contextStack = top;
}

//Adds a symbol to the table at the top of the stack
//name - Name of the variable to be inserted (will be used to find the symbol info)
//line - Line where the symbol was found
//type - type of the symbol
//nature - Nature of the symbol (don't ask me what it means, it should be type)
//fieldList - List of fields or arguments with the respective types, it should already be allocated or NULL
//vectorSize - If it is a vector, it should have its size, else it should be 1
//sizeOfString - If it is a string, it should have its size, else it should be 1
//returns 0 if the symbol exists
//returns 1 if it is inserted correctly
int addSymbol(char* name, int line, int type, int nature, struct fieldList* fields, int vectorSize, int sizeOfString, char* userType){
    if(findSymbolInCurrentContext(name) != NULL)
        return ERR_DECLARED;

    struct context* new = malloc(sizeof(struct context));

    //symbolInfo initialization
    new->info.line = line;
    new->info.nature = nature;
    new->info.type = type;
    new->info.size = getSizeFromType(type, vectorSize, sizeOfString);

    new->info.name = strdup(name);
    new->info.value.string_value = strdup(name);
    new->info.userType = NULL;
    new->info.fields = NULL;

    if(userType != NULL)
        new->info.userType = strdup(userType);

    if (fields != NULL)
        new->info.fields = fields;

    new->info.registerTemp = NULL;
    new->info.rfpOffset = -1;
    new->info.rbssOffset = -1;

    HASH_ADD_STR(contextStack->currentContext, info.name, new);

    return 0;
}

//Deletes current context, frees it and pops it from the stack
void deleteContext(){
    struct context* currentSymbol, *tmp;

    //Frees the hash of current context
    HASH_ITER(hh, contextStack->currentContext, currentSymbol, tmp) {
        HASH_DEL(contextStack->currentContext,currentSymbol);  /* delete; users advances to next */
        freeSymbolInfo(&(currentSymbol->info));            
        free(currentSymbol);
    }

    //Frees the top of the stack and moves the stack
    struct contextStack* top = contextStack;
    contextStack = contextStack->previousContext;
    rfpOffset = 0;
    free(top);

}

//Deletes current context, frees it and pops it from the stack
void deleteAllContext(){
    while(contextStack != NULL){
        struct context* currentSymbol, *tmp;

        //Frees the hash of current context
        HASH_ITER(hh, contextStack->currentContext, currentSymbol, tmp) {
            HASH_DEL(contextStack->currentContext,currentSymbol);  /* delete; users advances to next */
            freeSymbolInfo(&(currentSymbol->info));            
            free(currentSymbol);
        }

        //Frees the top of the stack and moves the stack
        struct contextStack* top = contextStack;
        contextStack = contextStack->previousContext;
        free(top);
    }
}

//Frees a symbol
void freeSymbolInfo(struct symbolInfo* info){
    free(info->name);
    if (info->value.string_value != NULL)
        free(info->value.string_value);
    
    if (info->userType != NULL)
        free(info->userType);
    
    if(info->fields != NULL)
    freeFieldList(info->fields);
}


//Given a FieldList, pushes a field into it and returns the new list
struct fieldList* pushField(struct fieldList* fieldList, int type, char* name, char* userType){
    if(fieldList == NULL){
        fieldList = malloc(sizeof(struct fieldList));
        fieldList->name = strdup(name);
        fieldList->type = type;
        if(type == NATUREZA_IDENTIFICADOR){
            fieldList->userType = strdup(userType);
        }
        else{
            fieldList->userType = NULL;
        }
        fieldList->next = NULL;
    }
    else{
        struct fieldList* new = malloc(sizeof(struct fieldList));
        new->name = strdup(name);
        new->type = type;
        new->userType = NULL;
        if(type == NATUREZA_IDENTIFICADOR){
            new->userType = strdup(userType);
        }
        new->next = NULL;

        struct fieldList* temp = fieldList;
        while(temp->next != NULL)
            temp = temp->next;
        temp->next = new;
    }
    return fieldList;
}

void freeFieldList(struct fieldList* fieldList){
    while(fieldList != NULL){
        free(fieldList->name);
        if(fieldList->userType != NULL){
            free(fieldList->userType);
        }
        struct fieldList* temp = fieldList;
        fieldList= fieldList->next;
        free(temp);
    }
}

//Find symbol in ALL contexts
//returnInfo - symbolInfo of the symbolFound or NULL
struct symbolInfo* findSymbolInContexts(char* name){
    struct contextStack* tempStack;
    struct context* tempContext = NULL;
    struct symbolInfo* returnInfo = NULL;

    tempStack = contextStack;
    
    while(tempStack != NULL){
        HASH_FIND_STR(tempStack->currentContext, name, tempContext);

        if (tempContext){
            returnInfo = &tempContext->info;
            break;
        };
        tempStack = tempStack->previousContext;
    }
    return returnInfo;
}

//Find symbol in current contexts
//returnInfo - symbolInfo of the symbolFound or NULL
struct symbolInfo* findSymbolInCurrentContext(char* name){
    struct context* tempContext = NULL;
    struct symbolInfo* returnInfo = NULL;

    
    HASH_FIND_STR(contextStack->currentContext, name, tempContext);

    if (tempContext){
      returnInfo = &tempContext->info;
      
    }

    return returnInfo;
}


int updateStringSizeOnNode(char* name, int stringSize){
    struct contextStack* tempStack;
    struct context* tempContext;
    struct symbolInfo returnInfo;

    struct context* new = malloc(sizeof(struct context));

    tempStack = contextStack;
    while(tempStack != NULL){
        HASH_FIND_STR(tempStack->currentContext, name, tempContext);

        if (tempContext){
            memcpy(new, tempContext, sizeof(struct context));
            new->info.size =  stringSize;

            HASH_DEL(tempStack->currentContext, tempContext);
            free(tempContext);

            HASH_ADD_STR(tempStack->currentContext, info.name, new);
            return 0;
        };
        tempStack = tempStack->previousContext;
    }
    return -1;
}

//checks if the fieldList has the name searched, and return its type.
//Returns -1 if the name searched is not on the fieldList
int searchFieldList(struct fieldList* fieldList, char* name, char* userType){
    struct fieldList* tmp = fieldList;
    while (tmp != NULL){
        if(!strcmp(tmp->name, name)){
            if(tmp->type == NATUREZA_IDENTIFICADOR){
                if(tmp->userType != NULL){
                    userType = tmp->userType;
                }
            }

            return tmp->type;
        }
        
        tmp = tmp->next;
    }
    return -1;
}




int findSymbolFunctionInCurrentContext(char ** userType){
    struct context* currentSymbol, *tmp;

    //Frees the hash of current context
    HASH_ITER(hh, contextStack->currentContext, currentSymbol, tmp) {
        if(currentSymbol->info.nature == NATUREZA_FUNC){
            if(currentSymbol->info.type == NATUREZA_IDENTIFICADOR){
                *userType = currentSymbol->info.userType;
            }
            return currentSymbol->info.type;
        }

    }
    return ERR_UNDECLARED;

}