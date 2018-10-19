#include "symbolTableInterface.h"

//given a node and its type, adds it to the current symbol table
void addSymbolFromNode(struct node* idNode, struct node* typeNode){
    int typeOfTypeNode = parseType(typeNode->token_value);

    addSymbol(idNode->token_value, idNode->line_number, typeOfTypeNode, typeOfTypeNode, NULL, 1, 1); 
}

//given a node and its type and a node with the vector size, adds it to the current symbol table
void addSymbolFromNodeWithVector(struct node* idNode, struct node* typeNode, struct node* vectorSizeNode){
    int typeOfTypeNode = parseType(typeNode->token_value);

    addSymbol(idNode->token_value, idNode->line_number, typeOfTypeNode, typeOfTypeNode, NULL, vectorSizeNode->value.int_value, 1); 
}

//given a node and its type, adds it to the current symbol table
void addSymbolFromNodeWithAttribution(struct node* idNode, struct node* typeNode, struct node* expressionNode){
    int typeOfTypeNode = parseType(typeNode->token_value);

    if(typeOfTypeNode == NATUREZA_LITERAL_STRING)
        addSymbol(idNode->token_value, idNode->line_number, typeOfTypeNode, typeOfTypeNode, NULL, 1, getAttributedStringSize(expressionNode) ); 
}

void addSymbolFromNodeWithFieldList(struct node* idNode, struct node* typeNode, struct node* fieldListNode){
    struct fieldList* fieldList = createFieldList(fieldListNode);

    int typeOfTypeNode = parseType(typeNode->token_value);
    addSymbol(idNode->token_value, idNode->line_number, typeOfTypeNode, typeOfTypeNode, NULL, 1, 1); 
}

//Given a node that contains fields or functionArgumentList
struct fieldList* createFieldList(struct node* fields){
    struct fieldList* fieldList;
    if(strcmp(fields->token_value,"fields") == 0){
        if(numberOfChildren(fields) == 3){
            fieldList = pushField(fieldList, parseType(fields->child->brother->token_value), fields->child->brother->brother->token_value);
        }
        else if(numberOfChildren(fields) == 5){
            struct node* nodeIterator = fields;
            while(nodeIterator != NULL){
                fieldList = pushField(fieldList, parseType(fields->child->brother->token_value), fields->child->brother->brother->token_value);
                nodeIterator = nodeIterator->child->brother->brother->brother->brother;
            }
        }
        
    }
    return fieldList;
}