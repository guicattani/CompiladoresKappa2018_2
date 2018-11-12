/*
Função principal para realização da E3.

Este arquivo não pode ser modificado.
*/
#include <stdio.h>
#include "parser.tab.h" //arquivo gerado com bison -d parser.y
#include "codeGen.h"

void *arvore = NULL;
void descompila (void *arvore);
void libera (void *arvore);

int main (int argc, char **argv)
{

  int ret = yyparse(); 

  return ret;
}