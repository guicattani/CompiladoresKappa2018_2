#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

#include "hashtable/uthash.h"
#include "auxiliar.h"


//FieldList - List of fields/Arguments and its respective types.
typedef struct fieldList{
    char* name;
    int type;
    struct fieldList* next;
} FIELDLIST;

typedef struct symbolInfo{
    int line;
    int nature;
    int type;
    int size;
    
    //Info from yylval
    char* name;
    union value value;

    struct fieldList* fields; //If this node is a function/Class
} SYMBOLINFO;

//currentContext - HashTable of a stack of names associated with their meaning and respective nodes on the ASTree
typedef struct context{

    struct symbolInfo info;    
    UT_hash_handle hh;


} CONTEXT;

typedef struct contextStack{
    struct context* currentContext;
    struct contextStack* previousContext; //next of the stack
} CONTEXTSTACK;

int addSymbol(char * name, int line, int type, int nature, struct fieldList* fields, int vectorSize, int sizeOfString);
void createContext();
void freeSymbolInfo(struct symbolInfo* info);
void deleteContext();
struct symbolInfo* findSymbolInContexts(char* name);
int setSymbolValue(char* name, char* value);

#endif