#ifndef SYMBOLTABLEINTERFACE
#define SYMBOLTABLEINTERFACE

#include "auxiliar.h"
#include "symbolTable.h"
#include "ASTree.h"

int addSymbolFromNode(struct node* idNode, struct node* typeNode);
int addSymbolFromNodeWithVector(struct node* idNode, struct node* typeNode, struct node* vectorSizeNode);
int addSymbolFromNodeWithAttribution(struct node* idNode, struct node* typeNode, struct node* expressionNode);
int  addSymbolFromNodeFunction(struct node* functionhead);
int  addSymbolFromNodeClass(struct node* idNode, struct node* fieldListNode);
int addSymbolFromLocalVarDeclaration(struct node *localVarCompleteDeclaration);

int checkAttribution(struct node* id, struct node* vector, struct node* expression, struct node* classid);
int checkPrimitiveAttribution(struct node* attrNode);
int checkUserTypeAttribution(struct node* attrNode);

struct fieldList* createFieldList(struct node* fields);
int calculateTypeInfer(struct node* node);

int isIdentifierOfNatureVector(struct node* node);
int isIdentifierOfNatureClassVector(struct node* node);
int isIdentifierDeclared(struct node* node);

int getTypeFromUserClassField(struct node* variableNode,struct node* fieldClassNode);


#endif