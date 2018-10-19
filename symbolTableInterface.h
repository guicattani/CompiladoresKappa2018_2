#ifndef SYMBOLTABLEINTERFACE
#define SYMBOLTABLEINTERFACE

#include "auxiliar.h"
#include "symbolTable.h"
#include "ASTree.h"

void addSymbolFromNode(struct node* idNode, struct node* typeNode);
void addSymbolFromNodeWithVector(struct node* idNode, struct node* typeNode, struct node* vectorSizeNode);
void addSymbolFromNodeWithAttribution(struct node* idNode, struct node* typeNode, struct node* expressionNode);

struct fieldList* createFieldList(struct node* fields);
void addSymbolFromNodeWithFieldList(struct node* idNode, struct node* typeNode, struct node* fieldListNode);


#endif