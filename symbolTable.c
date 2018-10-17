#include "symbolTable.h"
#include "hashtable/uthash.h"
#include "stdio.h"
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
//VectorSize - If it is a vector, it should have its size, else it should be 1
void addSymbol(char * name, int line, int type, int nature, struct fieldList* fields, int vectorSize){

    struct context* new = malloc(sizeof(struct context));
    new->info.line = line;
    new->info.nature = nature;
    new->info.type = type;
    new->info.size = getSizeFromType(type, vectorSize, name);

    new->info.name = _strdup(name);
    union value val = createValue(type, name);
    new->info.value = val;


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

