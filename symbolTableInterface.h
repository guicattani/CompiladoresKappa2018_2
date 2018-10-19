#ifndef SYMBOLTABLEINTERFACE
#define SYMBOLTABLEINTERFACE

#include "auxiliar.h"
#include "symbolTable.h"
#include "ASTree.h"

int addSymbolFromNode(struct node* idNode, struct node* typeNode);
int addSymbolFromNodeWithVector(struct node* idNode, struct node* typeNode, struct node* vectorSizeNode);
void addSymbolFromNodeWithAttribution(struct node* idNode, struct node* typeNode, struct node* expressionNode);
int  addSymbolFromNodeFunction(struct node* idNode, struct node* typeNode, struct node* fieldListNode);
int  addSymbolFromNodeClass(struct node* idNode, struct node* fieldListNode);


struct fieldList* createFieldList(struct node* fields);
#endif