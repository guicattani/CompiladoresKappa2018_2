#include "symbolTableInterface.h"

//given a node and its type, adds it to the current symbol table
//Returns 1 if succesful
//Returns 0 if symbol already exists
//stringSize is the size of the node if it is a striing
int addSymbolFromNode(struct node* idNode, struct node* typeNode){



    int typeOfTypeNode = parseType(typeNode->token_value);
    int nature = typeOfTypeNode;
    char* userType = NULL;
    if (typeOfTypeNode == NATUREZA_IDENTIFICADOR){

        userType = strdup(typeNode->token_value); 
        nature = NATUREZA_CLASSE;
    }

    if(findSymbolInContexts(typeNode->token_value) == NULL && typeOfTypeNode == NATUREZA_IDENTIFICADOR)
        return ERR_TYPE_UNDECLARED;

    if(addSymbol(idNode->token_value, idNode->line_number, typeOfTypeNode, nature, NULL, 1, 1, userType) == 0)
        return 0;
    else 
        return ERR_DECLARED; 

}

//given a node and its type and a node with the vector size, adds it to the current symbol table
int addSymbolFromNodeWithVector(struct node* idNode, struct node* typeNode, struct node* vectorSizeNode){
    int nature = NATUREZA_VETOR;
    int typeOfTypeNode = parseType(typeNode->token_value);
    if (typeOfTypeNode == NATUREZA_IDENTIFICADOR)
        nature = NATUREZA_VETOR_CLASSE;
    
    char* userType = NULL;
    if (typeOfTypeNode == NATUREZA_IDENTIFICADOR)
        userType = typeNode->token_value; 

    if(findSymbolInContexts(typeNode->token_value) == NULL && typeOfTypeNode == NATUREZA_IDENTIFICADOR)
        return ERR_TYPE_UNDECLARED;

    if(addSymbol(idNode->token_value, idNode->line_number, typeOfTypeNode, nature, NULL, vectorSizeNode->value.int_value, 1, userType) == 0)
        return 0;
    else 
        return ERR_DECLARED; 
}

//given a node and its type, adds it to the current symbol table 
int addSymbolFromNodeWithAttribution(struct node* idNode, struct node* typeNode, struct node* expressionNode){ //TODO UNUSED!!!
    int typeOfTypeNode = parseType(typeNode->token_value);

    if(findSymbolInContexts(typeNode->token_value) == NULL && typeOfTypeNode == NATUREZA_IDENTIFICADOR)
        return ERR_TYPE_UNDECLARED;

    if(typeOfTypeNode == NATUREZA_LITERAL_STRING){
        if(addSymbol(idNode->token_value, idNode->line_number, typeOfTypeNode, typeOfTypeNode, NULL, 1, -1, NULL) == 0)
            return 0;
        else
            return ERR_DECLARED; 

    }
    else{
        if(addSymbol(idNode->token_value, idNode->line_number, typeOfTypeNode, typeOfTypeNode, NULL, 1, 1, NULL) == 0)
            return 0;
        else
            return ERR_DECLARED; 
    }
}



int  addSymbolFromNodeFunction(struct node* functionhead){
    struct node* idNode, *typeNode, *fieldListNode;
    if (numberOfChildren(functionhead) == 5){
        idNode = functionhead->child->brother;
        typeNode = functionhead->child;
        fieldListNode = functionhead->child->brother->brother->brother;        

    }   
    else if (numberOfChildren(functionhead) == 6){
        idNode = functionhead->child->brother->brother;
        typeNode = functionhead->child->brother;
        fieldListNode = functionhead->child->brother->brother->brother->brother;        
    }

    struct fieldList* fieldList = createFieldList(fieldListNode);
    int typeOfTypeNode = parseType(typeNode->token_value);

    if(findSymbolInContexts(typeNode->token_value) == NULL && typeOfTypeNode == NATUREZA_IDENTIFICADOR)
        return ERR_TYPE_UNDECLARED;

    if (addSymbol(idNode->token_value, idNode->line_number, typeOfTypeNode, NATUREZA_FUNC, fieldList, 1, 1, NULL) == 0)
        return 0;
    else
        return ERR_DECLARED; 
}

int  addSymbolFromNodeClass(struct node* idNode, struct node* fieldListNode){
    struct fieldList* fieldList = createFieldList(fieldListNode);
    if (addSymbol(idNode->token_value, idNode->line_number, NATUREZA_IDENTIFICADOR, NATUREZA_CLASSE, fieldList, 1, 1, NULL) == 0)
        return 0;
    else
        return ERR_DECLARED; 
}

int addSymbolFromLocalVarDeclaration(struct node *localVarCompleteDeclaration){
    struct node* localVarDeclaration = localVarCompleteDeclaration->child;
    while(localVarDeclaration->brother != NULL)
        localVarDeclaration = localVarDeclaration->brother;
    
    struct node* typeNode = localVarDeclaration->child; 
    struct node* idNode = localVarDeclaration->child->brother;
    
    int typeOfTypeNode = parseType(typeNode->token_value);
    int nature = typeOfTypeNode;

    //Inserts UserType
    char* userType = NULL;
    if (typeOfTypeNode == NATUREZA_IDENTIFICADOR){

        userType = strdup(typeNode->token_value); 
        nature = NATUREZA_CLASSE;
    }

    //Checks if type has been declared
    if(findSymbolInContexts(typeNode->token_value) == NULL && typeOfTypeNode == NATUREZA_IDENTIFICADOR)
        return ERR_TYPE_UNDECLARED;

    int stringSize = -1; //TODO calculate true stringsize based on expression
    
    if(typeNode->token_type == NATUREZA_LITERAL_STRING){
        struct node* declarationNode = localVarDeclaration->child->brother->brother->child->brother->child;
        if(declarationNode->token_type == NATUREZA_IDENTIFICADOR){
            struct symbolInfo* typeInfo = findSymbolInContexts(declarationNode->token_value);
            stringSize = typeInfo->size;
        }
        else{
            stringSize = strlen(declarationNode->token_value);
        }
    }



    //Adds symbol and checks if identifier it has been declared before
    if(addSymbol(idNode->token_value, idNode->line_number, typeOfTypeNode, nature, NULL, 1, stringSize, userType) != 0)
        return ERR_DECLARED; 
    if(numberOfChildren(localVarDeclaration->child->brother->brother) != 0){
        //Attribution
        struct node* type = localVarDeclaration->child->brother->brother->child->brother; //Goes into userVariableOrLiteral Node
        int attrType = type->token_type;
        if(attrType == NATUREZA_IDENTIFICADOR){
            struct symbolInfo* typeInfo = findSymbolInContexts(type->token_value);
            if(typeInfo == NULL)
                return ERR_UNDECLARED;          
            attrType = typeInfo->type;
            if(typeInfo->nature == NATUREZA_CLASSE || typeInfo->nature == NATUREZA_VETOR_CLASSE) //TODO review
                return ERR_USER;
            if(typeInfo->nature == NATUREZA_FUNC)
                return ERR_FUNCTION;
        }

        int resultado = calculateImplicitConvert(attrType, typeOfTypeNode);

        printf("asdasdsa");
        if(resultado == -1)
            return ERR_WRONG_TYPE;
    }


    return 0;
    
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
                fieldList = pushField(fieldList, parseType(nodeIterator->child->brother->token_value), nodeIterator->child->brother->brother->token_value);
                if (numberOfChildren(nodeIterator) == 5){
                    nodeIterator = nodeIterator->child->brother->brother->brother->brother; }
                else nodeIterator = NULL;
            }
        }
        
    }

    return fieldList;

}



int checkAttribution(struct node* id, struct node* vector, struct node* expression, struct node* classid){
    struct symbolInfo* idInfo = findSymbolInContexts(id->token_value);
    if(idInfo == NULL)
        return ERR_UNDECLARED;
        
    if((idInfo->nature != NATUREZA_VETOR && idInfo->nature != NATUREZA_VETOR_CLASSE) && !isVectorEmpty(vector))
        return ERR_VARIABLE;

    if((idInfo->nature == NATUREZA_VETOR || idInfo->nature == NATUREZA_VETOR_CLASSE)  && isVectorEmpty(vector))
        return ERR_VECTOR;

    if(idInfo->nature == NATUREZA_FUNC)
        return ERR_FUNCTION;

    //User type has been defined but it hasn't been called correctly
    if((idInfo->nature == NATUREZA_CLASSE || idInfo->nature == NATUREZA_VETOR_CLASSE)  && classid == NULL)
        return ERR_USER;

    

    if((idInfo->nature != NATUREZA_CLASSE && idInfo->nature != NATUREZA_VETOR_CLASSE)  && classid != NULL)
        return ERR_CLASS;    

    int tested_type = idInfo->type;
    if(classid != NULL){
        struct symbolInfo *classInfo = findSymbolInContexts(idInfo->userType);
        
        int type = searchFieldList(classInfo->fields, classid->token_value);
        if (type == -1)
            return ERR_CLASS_ID_NOT_FOUND;
        tested_type = type; 
    }

    int typeInferenceOfExpression; 
    if(expression->child != NULL && expression->brother != NULL){//not a literal
        typeInferenceOfExpression = calculateTypeInfer(expression);
        
        if(typeInferenceOfExpression > NATUREZA_IDENTIFICADOR){
            return typeInferenceOfExpression;
        }
    }
    else{ //simple expression, has only one string
        if(expression->token_type == NATUREZA_IDENTIFICADOR){
            struct symbolInfo* referenceInfo = findSymbolInContexts(expression->token_value);
            typeInferenceOfExpression = referenceInfo->type;
        }
        else
            typeInferenceOfExpression = expression->token_type;
    }

    int calculatedConvert = calculateImplicitConvert(tested_type, typeInferenceOfExpression);
    if(calculatedConvert == -1)
        return ERR_WRONG_TYPE;
    
    if(calculatedConvert == NATUREZA_LITERAL_STRING){
        int idSize = idInfo->size;
        if(idInfo->size < 0){
            int stringSize;
            if(expression->token_type == NATUREZA_IDENTIFICADOR){
                struct symbolInfo* typeInfo = findSymbolInContexts(expression->token_value);
                stringSize = typeInfo->size;
                if(stringSize < 0)
                    return 0;
            }
            else{
                stringSize = strlen(expression->token_value) - 2; //2 because we have "" with the string
            }
            updateStringSizeOnNode(idInfo->name, stringSize);
        }
        else{
            int stringSize;
            if(expression->token_type == NATUREZA_IDENTIFICADOR){
                struct symbolInfo* typeInfo = findSymbolInContexts(expression->token_value);
                stringSize = typeInfo->size;
            }
            else{
                stringSize = strlen(expression->token_value) - 2; //2 because we have "" with the string
            }
            if(stringSize > idSize){
                return ERR_STR_SIZE_OVERFLOW;
            }
        }
    }

     return 0;

}

int checkPrimitiveAttribution(struct node* attrNode){
    struct node* id = attrNode->child;
    struct node* vector = attrNode->child->brother;
    struct node* expression = attrNode->child->brother->brother->brother;
    checkAttribution(id, vector, expression, NULL);
}

int checkUserTypeAttribution(struct node* attrNode){
    struct node* id = attrNode->child;
    struct node* vector = attrNode->child->brother;
    struct node* expression = attrNode->child->brother->brother->brother->brother->brother;
    struct node* typeid = attrNode->child->brother->brother->brother;
    checkAttribution(id, vector, expression, typeid);
}

//return a coerced type, based on expression and precedence
int calculateTypeInfer(struct node* node){
    if(node == NULL)
        return 0;
    if(strcmp(node->token_value , "$") == 0){
        int brotherInfer = calculateTypeInfer(node->brother->brother);
        
        return brotherInfer;
    }
        int childInfer = calculateTypeInfer(node->child);
        int brotherInfer = calculateTypeInfer(node->brother);

    if(childInfer > NATUREZA_IDENTIFICADOR) //propagate error msg;
        return childInfer;
    if(brotherInfer > NATUREZA_IDENTIFICADOR) //propagate error msg;
        return brotherInfer;

    int nodeType = node->token_type;

    //dereference variable
    if(nodeType == NATUREZA_IDENTIFICADOR){
        struct symbolInfo* referenceInfo = findSymbolInContexts(node->token_value);
        int referenceType = referenceInfo->type;

        if(referenceInfo->nature == NATUREZA_FUNC) 
             return ERR_FUNCTION;

        if(referenceInfo->nature == NATUREZA_CLASSE) {
            if(referenceInfo->userType == NULL)
                return ERR_WRONG_TYPE;
            if(node->brother == NULL || node->brother->brother == NULL ){
                return ERR_WRONG_TYPE; //generic error because we can't determine the column
            }
            
            int typeClassField = getTypeFromUserClassField(node, node->brother->brother);
            if(typeClassField > NATUREZA_IDENTIFICADOR)
                return ERR_CLASS_ID_NOT_FOUND;
            else
                referenceType = typeClassField;
        }
        
        nodeType = referenceType;
    }

    if(nodeType == NATUREZA_LITERAL_CHAR){
        return ERR_CHAR_TO_X; //error, since we can't convert them
    }
    if(nodeType == NATUREZA_LITERAL_STRING){
        return ERR_STRING_TO_X; //error, since we can't convert them
    }
    //if inference of child have been calculated, assume it as type
    if(childInfer != 0) 
        nodeType = childInfer;

    //if nodeType is not a literal type, ignore
    if(nodeType < NATUREZA_LITERAL_INT  || nodeType > NATUREZA_LITERAL_BOOL){
        return brotherInfer;
    }

    if(brotherInfer == NATUREZA_LITERAL_INT &&
       nodeType == NATUREZA_LITERAL_INT)
       return NATUREZA_LITERAL_INT;
       
    if(brotherInfer == NATUREZA_LITERAL_FLOAT &&
       nodeType == NATUREZA_LITERAL_FLOAT)
       return NATUREZA_LITERAL_FLOAT;
       
    if(brotherInfer == NATUREZA_LITERAL_BOOL &&
       nodeType == NATUREZA_LITERAL_BOOL)
       return NATUREZA_LITERAL_BOOL;
       
    if( (brotherInfer == NATUREZA_LITERAL_FLOAT &&
       nodeType == NATUREZA_LITERAL_INT) || 
       (brotherInfer == NATUREZA_LITERAL_INT &&
       nodeType == NATUREZA_LITERAL_FLOAT) )
       return NATUREZA_LITERAL_FLOAT;
       
    if((brotherInfer == NATUREZA_LITERAL_BOOL &&
       nodeType == NATUREZA_LITERAL_INT) || 
       (brotherInfer == NATUREZA_LITERAL_INT &&
       nodeType == NATUREZA_LITERAL_BOOL))
       return NATUREZA_LITERAL_INT;

    if((brotherInfer == NATUREZA_LITERAL_BOOL &&
       nodeType == NATUREZA_LITERAL_FLOAT) ||
        (brotherInfer == NATUREZA_LITERAL_FLOAT &&
       nodeType == NATUREZA_LITERAL_BOOL ))
       return NATUREZA_LITERAL_FLOAT;

    return nodeType;
}

//1 if true, 0 if not
int isIdentifierOfNatureVector(struct node* node){
    struct symbolInfo* typeInfo = findSymbolInContexts(node->token_value);

    if(typeInfo->nature == NATUREZA_VETOR)
        return 1;
    else 
        return 0;
}

//1 if true, 0 if not
int isIdentifierOfNatureClassVector(struct node* node){
    struct symbolInfo* typeInfo = findSymbolInContexts(node->token_value);

    if(typeInfo->nature == NATUREZA_VETOR_CLASSE)
        return 1;
    else 
        return 0;
}

//1 if true, 0 if not
int isIdentifierDeclared(struct node* node){
    struct symbolInfo* typeInfo = findSymbolInContexts(node->token_value);
    if(typeInfo)
        return 1;
    else
        return 0;
}

//return type if class field is found within class, if not, return -1
int getTypeFromUserClassField(struct node* variableNode,struct node* fieldClassNode){
    struct symbolInfo *idInfo = findSymbolInContexts(variableNode->token_value);

    if(idInfo->userType == NULL)
        return ERR_CLASS_ID_NOT_FOUND;
    
    struct symbolInfo *classInfo = findSymbolInContexts(idInfo->userType);
    int type = searchFieldList(classInfo->fields, fieldClassNode->token_value);

    if (type == -1)
            return ERR_CLASS_ID_NOT_FOUND;
    return type;
}