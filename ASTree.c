#include "ASTree.h"
struct nodeList* nodeList = NULL;
struct node* createChildren(struct node* parent, struct node* child){
     struct node* nodeIterator;
     
     if(parent->child != NULL){
         nodeIterator = parent->child;
         while(nodeIterator->brother != NULL){
             
             nodeIterator = nodeIterator->brother;
         }
         nodeIterator->brother = child;
     }
     else{
         parent->child = child;
    }
    return parent;
}

struct node* createNode(char* state){
    struct node* newNode = malloc(sizeof(struct node));
    newNode->token_value = strdup(state);
    newNode->brother = NULL;
	newNode->child = NULL; 
    newNode->value.string_value = NULL;
    newNode->token_type = 0;

    nodeList = insertList(nodeList, newNode);
    return newNode;
}

struct node* createLeaf(int line_number, int type, char* text){
    struct node *newNode = malloc(sizeof(struct node));
    newNode->line_number = line_number;
    newNode->token_type = type;
    newNode->token_value = strdup(text);
    newNode->brother = NULL;
    newNode->child = NULL;

    switch (type){
        case 1: //Reserved word
        case 2: //Special Char 
        case 3: //Compound Operator
        case 4: //Identifier
            newNode->value.string_value = strdup(text);
            break;
        case 5: //Lit int
            newNode->value.int_value = atoi(text);
            break;
        case 6: //Literal Bool
            if (text == "true")
                newNode->value.bool_value = 1;
            else if (text == "false")
                newNode->value.bool_value = 0;
            break;
        case 7: //Literal Float
            newNode->value.float_value = atof(text);
            break;
        case 8: //Literal Char
        case 9: //Literal String
            newNode->value.string_value = strdup(text);
            break;

    }
    nodeList = insertList(nodeList, newNode);
    return newNode;
}


void showTree(struct node* root)
{
	printf("$\n");
    showTreeRecursion(root, 0);
}
void showTreeRecursion(struct node* currentNode, int treeLevel)
{
    if(currentNode == NULL)
        return;
     
    if(currentNode->token_type != 0){
        if(!strcmp(currentNode->token_value, "}")){
            treeLevel--;
            for(int i = 0; i<treeLevel;i++)
                printf("\t");
        }

        printf("%s ",currentNode->token_value);

        if(!strcmp(currentNode->token_value, "}")){
            putchar('\n');
        }

        if(!strcmp(currentNode->token_value, "{")){
            putchar('\n');
            treeLevel++;

            for(int i = 0; i<treeLevel;i++)
                printf("\t");
        }
        if(!strcmp(currentNode->token_value, ";")){
            putchar('\n');
            for(int i = 0; i<treeLevel;i++)
                printf("\t");
        }
            
    }
    else{
       // printf("###%s###\n",currentNode->token_value);
    }
    
    showTreeRecursion(currentNode->child, treeLevel);
    showTreeRecursion(currentNode->brother, treeLevel); 
}

void liberaTree(struct node* node){
    
    if (node != NULL){
        liberaTree(node->child);
        free(node->token_value);
        if(node->token_type != 5 || node->token_type != 6 || node->token_type != 7 )
            //node has a string in it
            if (node->value.string_value != NULL){
                free(node->value.string_value); 
            }
        free(node);
    }
    
}


struct nodeList* insertList(struct nodeList* list, struct node* node){
    //printf("Inserindo %s\n",node->token_value);
    if (list == NULL){
        list = malloc(sizeof(struct nodeList));
        list->data = node;
        list->next = NULL; 
        
    }
    else{
        struct nodeList* currentNode = list;
        while(currentNode->next != NULL){
            currentNode = currentNode->next;
        }
        currentNode->next = malloc(sizeof(struct nodeList));
        currentNode = currentNode->next;
        currentNode->data = node;
        currentNode->next = NULL;

         // printf("Inserido %s na lista\n", currentNode->data->token_value);
    }
    return list;
}
void cleanList(struct nodeList* list){
    if (list == NULL)
        return;
    cleanList(list->next);

    //printf("limpando %s %d\n", list->data->token_value, list->data->token_type);
    if (list->data != NULL){
        
        if(list->data->token_type != 5 && list->data->token_type != 6 && list->data->token_type != 7 && list->data->token_type != 0)
            //node has a string in it
            //printf("Hey olha temos uma string em %s\n", list->data->token_value);
            if (list->data->value.string_value != NULL)
                free(list->data->value.string_value); 
        free(list->data->token_value);
        free(list->data);
    }
    
    free(list);

} 
