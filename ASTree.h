#ifndef ASTREEH
#define ASTREEH
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
union value{
        int     int_value;
        int     bool_value;
        float   float_value;
        char    char_value;
        char*   string_value;
    };
typedef struct node{
    int     line_number;
    int     token_type;
    char*   token_value;
    
    union value value;
    struct node* brother;
    struct node* child;
} NODE;

void showTreeRecursion(struct node* currentNode, int treeLevel);
struct node* createNodeOnYYVal(struct node* newNode);
struct node* createChildren(struct node* parent, struct node* child);
struct node* createNode(char* state);
void showTree(struct node* root);
struct node* createLeaf(int line_number, int type, char* text);
void liberaTree(struct node* node);
#endif