#include "symbolTable.h"

struct contextStack* contextStack = NULL;

//Create a new context for the currentContext by pushing a new symbol table
//Since it is a stack, the table has to be moved to the front of the stack
void createContext(){
    struct contextStack* top = malloc(sizeof(struct contextStack));
    top->currentContext = NULL;
    top->previousContext = contextStack;
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
int addSymbol(char* name, int line, int type, int nature, struct fieldList* fields, int vectorSize, int sizeOfString){
    struct context* new = malloc(sizeof(struct context));
    new->info.line = line;
    new->info.nature = nature;
    new->info.type = type;
    new->info.size = getSizeFromType(type, vectorSize, sizeOfString);

    new->info.name = strdup(name);
    new->info.value.string_value = strdup(name);

    if (fields != NULL)
        new->info.fields = fields;

    HASH_ADD_STR(contextStack->currentContext, info.name, new);
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
    free(top);

}

//Frees a symbol
void freeSymbolInfo(struct symbolInfo* info){
    free(info->name);
    if (info->type == NATUREZA_LITERAL_STRING)
        free(info->value.string_value);
        

}


//Given a FieldList, pushes a field into it and returns the new list
struct fieldList* pushField(struct fieldList* fieldList, int type, char* name){
    if(fieldList == NULL){
        fieldList = malloc(sizeof(struct fieldList));
        fieldList->name = strdup(name);
        fieldList->type = type;
        fieldList->next = NULL;
    }
    else{
        
        struct fieldList* new = malloc(sizeof(struct fieldList));
        new->name = strdup(name);
        new->type = type;
        

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
        struct fieldList* temp = fieldList;
        fieldList= fieldList->next;
        free(temp);
    }
}

struct symbolInfo* findSymbolInContexts(char* name){
    struct contextStack* tempStack;
    struct context* tempContext;
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

int setSymbolValue(char* name, char* value){
    struct contextStack* tempStack;
    struct context* tempContext;
    struct symbolInfo returnInfo;

    struct context* new = malloc(sizeof(struct context));

    tempStack = contextStack;
    while(tempStack != NULL){
        HASH_FIND_STR(tempStack->currentContext, name, tempContext);

        if (tempContext){
            memcpy(new, tempContext, sizeof(struct context));
            union value val = createValue(new->info.type, value);
            new->info.value = val;

            HASH_DEL(tempStack->currentContext, tempContext);
            free(tempContext);

            HASH_ADD_STR(tempStack->currentContext, info.name, new);
            return 0;
        };
        tempStack = tempStack->previousContext;
    }
    return -1;
}
