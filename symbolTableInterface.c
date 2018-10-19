#include "symbolTableInterface.h"

//given a node and its type, adds it to the current symbol table
//Returns 1 if succesful
//Returns 0 if symbol already exists
int addSymbolFromNode(struct node* idNode, struct node* typeNode){



    int typeOfTypeNode = parseType(typeNode->token_value);

    if(findSymbolInContexts(typeNode->token_value) == NULL)
        return ERR_TYPE_UNDECLARED;

    if(addSymbol(idNode->token_value, idNode->line_number, typeOfTypeNode, typeOfTypeNode, NULL, 1, 1))
        return 0;
    else 
        return ERR_DECLARED; 

}

//given a node and its type and a node with the vector size, adds it to the current symbol table
int addSymbolFromNodeWithVector(struct node* idNode, struct node* typeNode, struct node* vectorSizeNode){
    int typeOfTypeNode = parseType(typeNode->token_value);

    if(findSymbolInContexts(typeNode->token_value) == NULL)
        return ERR_TYPE_UNDECLARED;

    if(addSymbol(idNode->token_value, idNode->line_number, typeOfTypeNode, NATUREZA_VETOR, NULL, vectorSizeNode->value.int_value, 1))
        return 0;
    else 
        return ERR_DECLARED; 
}

//given a node and its type, adds it to the current symbol table
void addSymbolFromNodeWithAttribution(struct node* idNode, struct node* typeNode, struct node* expressionNode){
    int typeOfTypeNode = parseType(typeNode->token_value);

    if(typeOfTypeNode == NATUREZA_LITERAL_STRING)
        addSymbol(idNode->token_value, idNode->line_number, typeOfTypeNode, typeOfTypeNode, NULL, 1, getAttributedStringSize(expressionNode) ); 
    else
        addSymbol(idNode->token_value, idNode->line_number, typeOfTypeNode, typeOfTypeNode, NULL, 1, 1 ); 
}



int  addSymbolFromNodeFunction(struct node* idNode, struct node* typeNode, struct node* fieldListNode){
    struct fieldList* fieldList = createFieldList(fieldListNode);
    int typeOfTypeNode = parseType(typeNode->token_value);

    if(findSymbolInContexts(typeNode->token_value) == NULL)
        return ERR_TYPE_UNDECLARED;

    if (addSymbol(idNode->token_value, idNode->line_number, typeOfTypeNode, NATUREZA_FUNC, fieldList, 1, 1) == 1)
        return 0;
    else
        return ERR_DECLARED; 
}

int  addSymbolFromNodeClass(struct node* idNode, struct node* fieldListNode){
    struct fieldList* fieldList = createFieldList(fieldListNode);
    if (addSymbol(idNode->token_value, idNode->line_number, NATUREZA_IDENTIFICADOR, NATUREZA_CLASSE, fieldList, 1, 1) == 1)
        return 0;
    else
        return ERR_DECLARED; 
}

//Given a node that contains fields or functionHead
struct fieldList* createFieldList(struct node* fields){
    struct fieldList* fieldList = NULL;
    if(strcmp(fields->token_value,"Estado fields") == 0 || strcmp(fields->token_value, "Estado functionArgumentsList")){
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