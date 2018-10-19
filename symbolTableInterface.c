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