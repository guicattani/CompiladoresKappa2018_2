#ifndef CODEGENH
#define CODEGENH

#include <string.h>
#include "auxiliar.h"
#include "ASTree.h"
#include "symbolTable.h"

void updateNodeCodeOPERATION(struct node* topNode, struct node* leftOperand, struct node* rightOperand, struct node* operatorNode);
void updateNodeCodeDECLARATION(struct node* topNode, struct node* leftOperand, struct node* rightOperand);
void updateNodeCodeATTRIBUTION(struct node* topNode, struct node* leftOperand, struct node* rightOperand);

void updateNodeCodeLOCALDECLARATION(struct node* topNode, struct node* identifierNode, struct node* typeNode);
void updateNodeCodeGLOBALDECLARATION(struct node* topNode, struct node* identifierNode, struct node* typeNode);


void printCode(struct node* topNode);
char* calculateCodeGenValue(struct node* node);

struct code* concatTwoCodes(struct node* executedFirst, struct node* executedSecond);

#endif