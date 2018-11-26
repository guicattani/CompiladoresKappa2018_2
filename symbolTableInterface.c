#include "symbolTableInterface.h"

struct field field;
extern int rfpOffset;

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
        {
            if(userType != NULL)
                free(userType);
            return ERR_TYPE_UNDECLARED; }

    int err = addSymbol(idNode->token_value, idNode->line_number, typeOfTypeNode, nature, NULL, 1, 1, userType);
    
    if(userType != NULL)
        free(userType);

    return err;
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

    char* userType = NULL;
    if (typeOfTypeNode == NATUREZA_IDENTIFICADOR){
        userType = typeNode->token_value; 
    }

    if(findSymbolInContexts(typeNode->token_value) == NULL && typeOfTypeNode == NATUREZA_IDENTIFICADOR)
        return ERR_TYPE_UNDECLARED;

    
    if (addSymbol(idNode->token_value, idNode->line_number, typeOfTypeNode, NATUREZA_FUNC, fieldList, 1, 1, userType) == 0){
        return 0;
    }
    else
        return ERR_DECLARED; 
}

int  addSymbolFromNodeClass(struct node* idNode, struct node* fieldListNode){
    struct fieldList* fieldList = createFieldList(fieldListNode);
    if (addSymbol(idNode->token_value, idNode->line_number, NATUREZA_IDENTIFICADOR, NATUREZA_CLASSE, fieldList, 1, 1, NULL) == 0)
        return 0;
    else{
        freeFieldList(fieldList);
        return ERR_DECLARED;
    }

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

        userType = typeNode->token_value; 
        nature = NATUREZA_CLASSE;
    }

    struct symbolInfo* typeInfo = findSymbolInContexts(typeNode->token_value);
    //Checks if type has been declared
    if(typeInfo == NULL && typeOfTypeNode == NATUREZA_IDENTIFICADOR)
        return ERR_TYPE_UNDECLARED;

    if(typeInfo != NULL && typeOfTypeNode == NATUREZA_IDENTIFICADOR){
        if(typeInfo->nature == NATUREZA_FUNC)
            return ERR_FUNCTION;
        if(typeInfo->nature != NATUREZA_CLASSE && typeInfo->nature != NATUREZA_VETOR_CLASSE)
            return ERR_CLASS;

    }

    int stringSize = -1; 

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
    
    if(numberOfChildren(localVarDeclaration) == 3){
        if(numberOfChildren(localVarDeclaration->child->brother->brother) != 0){
            //Attribution
            
            struct node* type = localVarDeclaration->child->brother->brother->child->brother->child; //Goes into userVariableOrLiteral Node
            int attrType = type->token_type;
            if(attrType == NATUREZA_IDENTIFICADOR){
                struct symbolInfo* typeInfo = findSymbolInContexts(type->token_value);
                if(typeInfo == NULL)
                    return ERR_UNDECLARED;          
                attrType = typeInfo->type;
                if(typeInfo->nature == NATUREZA_CLASSE || typeInfo->nature == NATUREZA_VETOR_CLASSE)
                    return ERR_USER;
                if(typeInfo->nature == NATUREZA_FUNC){
                    return ERR_FUNCTION;
                }
            }

            int resultado = calculateImplicitConvert(attrType, typeOfTypeNode);
            if(resultado == -1)
                return ERR_WRONG_TYPE;
        }
    }


    return 0;
    
}

//Given a node that contains fields or functionHead
struct fieldList* createFieldList(struct node* fields){
    struct fieldList* fieldList = NULL;
    if(strcmp(fields->token_value,"Estado fields") == 0 || strcmp(fields->token_value, "Estado functionArgumentsList")){
        if(numberOfChildren(fields) == 3){
            fieldList = pushField(fieldList, parseType(fields->child->brother->token_value), fields->child->brother->brother->token_value, fields->child->brother->token_value);

        }
        else if(numberOfChildren(fields) == 5){
            struct node* nodeIterator = fields;
            while(nodeIterator != NULL){
                fieldList = pushField(fieldList, parseType(nodeIterator->child->brother->token_value), nodeIterator->child->brother->brother->token_value, nodeIterator->child->brother->token_value);
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

    if(idInfo->nature == NATUREZA_FUNC){
        return ERR_FUNCTION;
    }

    //User type has been defined but it hasn't been called correctly
    if((idInfo->nature == NATUREZA_CLASSE || idInfo->nature == NATUREZA_VETOR_CLASSE)  && classid == NULL)
        return ERR_USER;

    

    if((idInfo->nature != NATUREZA_CLASSE && idInfo->nature != NATUREZA_VETOR_CLASSE)  && classid != NULL)
        return ERR_CLASS;    

    int tested_type = idInfo->type;
    char* userTypeField = NULL;
    if(classid != NULL){
        struct symbolInfo *classInfo = findSymbolInContexts(idInfo->userType);
        
        int type = searchFieldList(classInfo->fields, classid->token_value, userTypeField);
        if (type == -1)
            return ERR_CLASS_ID_NOT_FOUND;
        tested_type = type; 
    }

    int typeInferenceOfExpression; 
    char* userTypeTypeInfer = NULL;
    if(expression->child != NULL || expression->brother != NULL){//not a literal

        typeInferenceOfExpression = calculateTypeInfer(expression, &userTypeTypeInfer, -1);

        if(typeInferenceOfExpression > NATUREZA_IDENTIFICADOR){
            return typeInferenceOfExpression;
        }
    }
    else{ //simple expression, has only one string
        if(expression->token_type == NATUREZA_IDENTIFICADOR){
            struct symbolInfo* referenceInfo = findSymbolInContexts(expression->token_value);
            typeInferenceOfExpression = referenceInfo->type;
        }
        else{
            typeInferenceOfExpression = calculateTypeInfer(expression, &userTypeTypeInfer, -1);
        }

        if(typeInferenceOfExpression > NATUREZA_IDENTIFICADOR){
            return typeInferenceOfExpression;
        }
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

    return checkAttribution(id, vector, expression, NULL);
}

int checkUserTypeAttribution(struct node* attrNode){
    struct node* id = attrNode->child;
    struct node* vector = attrNode->child->brother;
    struct node* expression = attrNode->child->brother->brother->brother->brother->brother;
    struct node* typeid = attrNode->child->brother->brother->brother;
    
    return checkAttribution(id, vector, expression, typeid);
}

int calculateTypeInfer(struct node* node, char** userType, int typeOfAttribution){
    if(node->typeInfered > 0){
        return node->typeInfered;
    }
    return calculateTypeInferRecursion(node, typeOfAttribution);
}


//return a coerced type, based on expression and precedence
int calculateTypeInferRecursion(struct node* node, int typeOfAttribution){
    if(node == NULL)
        return 0;
    if(strcmp(node->token_value , ",") == 0){
        return 0;
    }

    if(strcmp(node->token_value , "$") == 0){
        int brotherInfer = calculateTypeInferRecursion(node->brother->brother, typeOfAttribution);
        
        return brotherInfer;
    }

    struct symbolInfo* referenceInfo;
    if(node->token_type == NATUREZA_IDENTIFICADOR){

        referenceInfo = findSymbolInContexts(node->token_value);

        if(referenceInfo == NULL)
            return ERR_UNDECLARED;
        
        int functionNodeType = referenceInfo->type;

        if(referenceInfo->nature == NATUREZA_FUNC){
            if(node->brother != NULL) {
                if(strcmp(node->brother->token_value , "(") != 0)
                    return ERR_FUNCTION;
                else
                    node = node->brother;
                while(node->brother != NULL){
                    node = node->brother;
                    if(strcmp(node->brother->token_value , ")") == 0){
                        node = node->brother;
                        break;
                    }
                }
            }
            else {
                return ERR_FUNCTION;
            }
            node->token_type = functionNodeType;
        }

    }
    
    int childInfer = calculateTypeInferRecursion(node->child, typeOfAttribution);
    int brotherInfer = calculateTypeInferRecursion(node->brother, typeOfAttribution);


    if(childInfer == NATUREZA_LITERAL_CHAR){
        return ERR_CHAR_TO_X; //error, since we can't convert them
    }
    if(brotherInfer == NATUREZA_LITERAL_CHAR ){
        return ERR_CHAR_TO_X; 
    }
    if(brotherInfer == NATUREZA_LITERAL_STRING){
        return ERR_STRING_TO_X; 
    }
    if(childInfer == NATUREZA_LITERAL_STRING){
        return ERR_STRING_TO_X;
    }

    if(childInfer > NATUREZA_IDENTIFICADOR) //propagate error msg;
        return childInfer;
    if(brotherInfer > NATUREZA_IDENTIFICADOR) //propagate error msg;
        return brotherInfer;

    int nodeType = node->token_type;

    if(nodeType == NATUREZA_LITERAL_CHAR && typeOfAttribution != -1 && typeOfAttribution != NATUREZA_LITERAL_CHAR){
        return ERR_CHAR_TO_X;
    }        
        
    if(nodeType == NATUREZA_LITERAL_STRING && typeOfAttribution != -1 && typeOfAttribution != NATUREZA_LITERAL_STRING){
        return ERR_STRING_TO_X;
    }

    //dereference variable
    if(nodeType == NATUREZA_IDENTIFICADOR){
        referenceInfo = findSymbolInContexts(node->token_value);
        int referenceType = referenceInfo->type;

        if(referenceInfo->nature == NATUREZA_FUNC) {
            referenceInfo = findSymbolInContexts(referenceInfo->name);
            return referenceInfo->type;
        }

        if(referenceInfo->nature == NATUREZA_CLASSE) {
            if(referenceInfo->userType == NULL){
                return ERR_WRONG_TYPE;
            }
            if(node->brother == NULL || node->brother->brother == NULL ){
                return referenceType;
            }

            struct symbolInfo *classInfo = findSymbolInContexts(referenceInfo->userType);

            int isClassFieldDeclared = searchFieldList(classInfo->fields, node->brother->brother->token_value, referenceInfo->userType);
            if (isClassFieldDeclared == -1)
                return ERR_CLASS_ID_NOT_FOUND;
                
            int typeClassField = getTypeFromUserClassField(node, node->brother->brother);
            if(typeClassField > NATUREZA_IDENTIFICADOR)
                return ERR_CLASS_ID_NOT_FOUND;
            else
                referenceType = typeClassField;
        }
        
        nodeType = referenceType;
    }
    
   
    //if inference of child has been calculated, assume it as type
    if(childInfer != 0) {
        nodeType = childInfer;
    }
    //if nodeType is not a literal type, ignore
    if(nodeType < NATUREZA_LITERAL_INT  || nodeType > NATUREZA_LITERAL_BOOL){
        if(nodeType == NATUREZA_IDENTIFICADOR && referenceInfo->userType)
            return NATUREZA_IDENTIFICADOR;
        return brotherInfer;
    }

    int precedenceConversion = -1;

    if(brotherInfer == NATUREZA_LITERAL_INT &&
       nodeType == NATUREZA_LITERAL_INT){
       precedenceConversion = NATUREZA_LITERAL_INT;
    }
       
    if(brotherInfer == NATUREZA_LITERAL_FLOAT &&
       nodeType == NATUREZA_LITERAL_FLOAT){
        precedenceConversion = NATUREZA_LITERAL_FLOAT;
    }
       
    if(brotherInfer == NATUREZA_LITERAL_BOOL &&
       nodeType == NATUREZA_LITERAL_BOOL){
        precedenceConversion = NATUREZA_LITERAL_BOOL;
    }
       
    if( (brotherInfer == NATUREZA_LITERAL_FLOAT &&
       nodeType == NATUREZA_LITERAL_INT) || 
       (brotherInfer == NATUREZA_LITERAL_INT &&
       nodeType == NATUREZA_LITERAL_FLOAT) ) {
       precedenceConversion = NATUREZA_LITERAL_FLOAT;
    }
       
    if((brotherInfer == NATUREZA_LITERAL_BOOL &&
       nodeType == NATUREZA_LITERAL_INT) || 
       (brotherInfer == NATUREZA_LITERAL_INT &&
       nodeType == NATUREZA_LITERAL_BOOL)) {
       precedenceConversion = NATUREZA_LITERAL_INT;
    }

    if((brotherInfer == NATUREZA_LITERAL_BOOL &&
       nodeType == NATUREZA_LITERAL_FLOAT) ||
        (brotherInfer == NATUREZA_LITERAL_FLOAT &&
       nodeType == NATUREZA_LITERAL_BOOL )) {
       precedenceConversion = NATUREZA_LITERAL_FLOAT;
    }


    if(precedenceConversion > 0){
        node->implicitConversion = returnImplicitConversionCode(nodeType, typeOfAttribution);
        return precedenceConversion;
    }
    return nodeType;
}

int returnImplicitConversionCode(int attributionType, int literalType){
    if(attributionType < 0)
        return -1;

    if(literalType == NATUREZA_LITERAL_INT){
        if(attributionType == NATUREZA_LITERAL_INT)
            return 0;
        if(attributionType == NATUREZA_LITERAL_FLOAT)
            return IMPLICIT_CONVERT_INT_TO_FLOAT;
        if(attributionType == NATUREZA_LITERAL_BOOL)
            return IMPLICIT_CONVERT_INT_TO_BOOL;
    }
    if(literalType == NATUREZA_LITERAL_FLOAT){
        if(attributionType == NATUREZA_LITERAL_INT)
            return IMPLICIT_CONVERT_FLOAT_TO_INT;
        if(attributionType == NATUREZA_LITERAL_FLOAT)
            return 0;
        if(attributionType == NATUREZA_LITERAL_BOOL)
            return IMPLICIT_CONVERT_FLOAT_TO_BOOL;
    }
    if(literalType == NATUREZA_LITERAL_BOOL){
        if(attributionType == NATUREZA_LITERAL_INT)
            return IMPLICIT_CONVERT_BOOL_TO_INT;
        if(attributionType == NATUREZA_LITERAL_FLOAT)
            return IMPLICIT_CONVERT_BOOL_TO_FLOAT;
        if(attributionType == NATUREZA_LITERAL_BOOL)
            return 0;
    }
    return -1;
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
    char* userType = NULL;
    int type = searchFieldList(classInfo->fields, fieldClassNode->token_value, userType);

    if (type == -1)
            return ERR_CLASS_ID_NOT_FOUND;
    return type;

}

int checkFunctionPipe(struct node *functionPipeNode){

    if(!strcmp(functionPipeNode->child->token_value, AST_PIPECOMMANDS)){
        int err = checkFunctionPipe(functionPipeNode->child);
        if (err) return err;


        err = checkFunction(functionPipeNode->child->brother->brother, field.type, field.userType);
        if (err) return err;

        struct symbolInfo * funcInfo = findSymbolInContexts(functionPipeNode->child->brother->brother->child->token_value);
        if(funcInfo == NULL)
            return ERR_UNDECLARED;
        field.type = funcInfo->type;
        field.userType = funcInfo->userType;

        return 0;
        


    }else if(!strcmp(functionPipeNode->child->token_value, AST_FUNCTIONCALL)){
        struct symbolInfo *funcInfo = findSymbolInContexts(functionPipeNode->child->child->token_value);
        if(funcInfo == NULL)
            return ERR_UNDECLARED;

        int err = checkFunction(functionPipeNode->child, -1, NULL);
        if (err) return err;

        int funcType = funcInfo->type;
        char* funcUserType = funcInfo->userType;
        err = checkFunction(functionPipeNode->child->brother->brother, funcType, funcUserType);
        if (err) return err;

        funcInfo = findSymbolInContexts(functionPipeNode->child->brother->brother->child->token_value);
        if(funcInfo == NULL)
            return ERR_UNDECLARED;

        field.type = funcInfo->type;
        field.userType = funcInfo->userType;
        
        return 0;

    }

}

int checkFunction(struct node *functionNode, int type, char *userType){
    struct node* idNode = functionNode->child;

    struct node* functionCallArgument = functionNode->child;
    struct symbolInfo* idInfo = findSymbolInContexts(idNode->token_value);
    if(idInfo == NULL)
        return ERR_UNDECLARED;
    if(idInfo->nature == NATUREZA_CLASSE)
        return ERR_USER;
    
    if(idInfo->nature == NATUREZA_IDENTIFICADOR)
        return ERR_CLASS;
    
    if(idInfo->nature == NATUREZA_VETOR || idInfo->nature == NATUREZA_VETOR_CLASSE)
        return ERR_VECTOR;

    if(idInfo->nature != NATUREZA_FUNC)
        return ERR_VARIABLE;
    
    if(idInfo->nature == NATUREZA_FUNC){ 
        struct fieldList* field = idInfo->fields;
        
        struct node* functionCallArguments = functionNode->child->brother->brother;
        
        if(field == NULL && functionCallArguments->child != NULL)
            return ERR_EXCESS_ARGS;
        
        if(field != NULL && functionCallArguments->child == NULL)
            return ERR_MISSING_ARGS;

        if(field == NULL && functionCallArguments->child == NULL)
            return 0; 
        //both are not NULL
        struct node* functionCallArgumentList = functionCallArguments->child;
        while(field != NULL && functionCallArgumentList != NULL){
            int fieldType = field->type;
            //Gets the expression Type
            if(functionCallArgumentList == NULL)
                return ERR_MISSING_ARGS;

            struct node* argument = functionCallArgumentList->child;
            int expressionType = ERR_UNKNOWN;
            char* expressionName = NULL;
            if(!strcmp(argument->token_value, ".") && type != -1){
                expressionType = type;
                if(userType != NULL)
                    expressionName = userType;
            }
            else if(!strcmp(argument->token_value, ".") && type == -1)
                return ERR_WRONG_TYPE_ARGS;
            else{
                if(numberOfChildren(argument)){
                    char* userType = NULL;
                    expressionType = calculateTypeInfer(argument, &userType, -1);
                    if(expressionType == NATUREZA_IDENTIFICADOR && fieldType == NATUREZA_IDENTIFICADOR){
                        expressionName = field->userType;
                    }
                    if(userType != NULL)
                        free(userType);
                }
            }
            if(expressionType > 6)
                return expressionType;
            if(expressionType != fieldType){
                return ERR_WRONG_TYPE_ARGS;
            }

            if(numberOfChildren(functionCallArgumentList) == 1)
                functionCallArgumentList = NULL;
            else functionCallArgumentList = functionCallArgumentList->child->brother->brother;

            if (field->type == NATUREZA_IDENTIFICADOR && expressionType == NATUREZA_IDENTIFICADOR){
                if(strcmp(field->userType, expressionName))
                    return ERR_WRONG_TYPE_ARGS;
            }
            
            field = field->next;
        }

        if(field == NULL && functionCallArgumentList == NULL)
            return 0;
        if(field == NULL && functionCallArgumentList != NULL)
            return ERR_EXCESS_ARGS;
        if(field != NULL && functionCallArgumentList == NULL)
            return ERR_MISSING_ARGS;        

        return 0;
    }
}

void insertSymbolsFunction(struct node *function){
    char* name;
    if (numberOfChildren(function) == 5){
        name = function->child->brother->token_value;      

    }   
    else if (numberOfChildren(function) == 6){
        name = function->child->brother->brother->token_value;
    }
    struct symbolInfo* idInfo = findSymbolInContexts(function->child->token_value);
    
    struct fieldList* fieldList = idInfo->fields;
    while(fieldList != NULL){
        addSymbol(fieldList->name, idInfo->line, fieldList->type, fieldList->type, NULL, 1, -1, NULL);
        fieldList = fieldList->next;
    }

}

int addFunctionFields(struct node* functionHead){
    struct node* functionArgumentsList;
    rfpOffset = 12;
    if(numberOfChildren(functionHead) == 5){
        functionArgumentsList = functionHead->child->brother->brother->brother;
    } else if(numberOfChildren(functionHead) == 6)
        functionArgumentsList = functionHead->child->brother->brother->brother->brother;
    
    
    if(!strcmp(functionArgumentsList->token_value, AST_FUNCARGLIST))
        return 0;
    else if(!strcmp(functionArgumentsList->token_value, AST_FUNCARGELEM)){
        struct node* functionArgumentElements = functionArgumentsList;

    
        while(numberOfChildren(functionArgumentElements) == 5){
            int err = addSymbolFromNode(functionArgumentElements->child->brother->brother, functionArgumentElements->child->brother);
            if(err)
                return err;
            
            //add reg temp
            struct symbolInfo* info = findSymbolInCurrentContext(functionArgumentElements->child->brother->brother->token_value);
            info->registerTemp = newRegister();
            info->rfpOffset = rfpOffset;
            rfpOffset += 4;

            functionArgumentElements = functionArgumentElements->child->brother->brother->brother->brother;
        }
        int err = addSymbolFromNode(functionArgumentElements->child->brother->brother, functionArgumentElements->child->brother);
        
        //add reg temp
        struct symbolInfo* info = findSymbolInCurrentContext(functionArgumentElements->child->brother->brother->token_value);
        info->registerTemp = newRegister();
        info->rfpOffset = rfpOffset;
        rfpOffset += 4;

        if(err)
            return err;
    }

    return 0;
}

int checkOutputExpressionList(struct node* expressionList){
    while(expressionList != NULL){
        int err = checkOutputExpression(expressionList->child);
        if(err)
            return err;

        if(numberOfChildren(expressionList) == 3)
            expressionList = expressionList->child->brother->brother;
        else expressionList = NULL;
    }
    return 0;

}

int checkOutputExpression(struct node* expression){
    if(expression->child->token_type == NATUREZA_LITERAL_STRING){
        return 0;
    }
    int type = calculateTypeInfer(expression, NULL, NATUREZA_LITERAL_INT);
    if(type > 6)
        return type;

    if(type == NATUREZA_LITERAL_INT || type == NATUREZA_LITERAL_FLOAT)
        return 0;
    else return ERR_WRONG_PAR_OUTPUT;
     
}

