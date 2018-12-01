#ifndef CODEGENH
#define CODEGENH

#include <string.h>
#include "auxiliar.h"
#include "ASTree.h"
#include "symbolTable.h"

void updateNodeCodeOPERATION(struct node* topNode, struct node* leftOperand, struct node* rightOperand, struct node* operatorNode);
void updateNodeCodeDECLARATION(struct node* topNode, struct node* leftOperand, struct node* rightOperand);
struct code* updateNodeCodeATTRIBUTION(struct node* topNode, struct node* leftOperand, struct node* rightOperand);

void updateNodeCodeARITHCOMPARISON(struct node* topNode, struct node* leftOperand, struct node* rightOperand, struct node* operatorNode);

void updateNodeCodeLOCALDECLARATION(struct node* topNode, struct node* identifierNode, struct node* typeNode);
void updateNodeCodeGLOBALDECLARATION(struct node* topNode, struct node* identifierNode, struct node* typeNode);

void updateNodeCodeIF(struct node* ifNode, struct node* condition, struct node* ifTrue);
void updateNodeCodeIFELSE(struct node* ifNode, struct node* condition, struct node* ifTrue, struct node* ifFalse);
void updateCodeDOWHILE(struct node* whileNode, struct node* condition, struct node* commandsBlock);
void updateCodeWHILE(struct node* whileNode, struct node* condition, struct node* commandsBlock);

void printCode(struct node* topNode);
char* calculateCodeGenValue(struct node* node);

struct code* concatTwoCodes(struct code* executedFirst, struct code* executedSecond);
struct code* getNextLine(struct code* code);
void fixLine(char* line, int par, char* replacement);

struct code* makeReturnCode(struct node* expressionNode);
int getParameterCount(struct node* functionDeclaration);
struct code* endFunctionCode(struct node* functionHead, struct node* functionCommandsBlock);

void liberaCode(struct code* code);

struct code* removeCBR(struct code* code);
void patching(struct code* code, char* replacement, int par);

struct code* addJumpToFirstLine(struct code* program);
void declareFunctionCode(struct node* functionDeclaration);
struct code* writeFunctionCall(struct node* functionCall);

#endif