/*
Função principal para realização da E3.

Este arquivo não pode ser modificado.
*/
#include <stdio.h>
#include "parser.tab.h" //arquivo gerado com bison -d parser.y
#include "symbolTable.h"
void *arvore = NULL;
void descompila (void *arvore);
void libera (void *arvore);
extern struct contextStack* contextStack;
int main (int argc, char **argv)
{
    createContext();
    addSymbol( "oi", 2, NATUREZA_LITERAL_STRING, NATUREZA_LITERAL_STRING, NULL, 1, "3");
    addSymbol( "oi2", 2, NATUREZA_LITERAL_STRING, NATUREZA_LITERAL_STRING, NULL, 1, "3");
    struct symbolInfo* newInfo = findSymbolInContexts("oi");
    printf("a%s", newInfo->value.string_value);

    createContext();
    addSymbol( "oi", 2, NATUREZA_LITERAL_STRING, NATUREZA_LITERAL_STRING, NULL, 1, "4");
    newInfo = findSymbolInContexts("oi");
    printf("b%s", newInfo->value.string_value);
    newInfo = findSymbolInContexts("oi2");
    printf("c%s", newInfo->value.string_value);

    setSymbolValue("oi", "5");
    newInfo = findSymbolInContexts("oi");
    printf("b%s", newInfo->value.string_value);

    return 0;

}