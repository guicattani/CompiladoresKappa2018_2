#ifndef ASTREEH
#define ASTREEH
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "auxiliar.h"

extern struct nodeList* nodeList;

struct nodeList* insertList(struct nodeList* list, struct node* node);
void cleanList(struct nodeList* node); 


void showTreeRecursion(struct node* currentNode, int treeLevel);
struct node* createNodeOnYYVal(struct node* newNode);
struct node* createChildren(struct node* parent, struct node* child);
struct node* createNode(char* state);
void showTree(struct node* root);
struct node* createLeaf(int line_number, int type, char* text);
void liberaTree(struct node* node);
#endif