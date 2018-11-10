#include "codeGen.h"

int registerIndex = 0;
int labelIndex = 0;

//rfpOffset have been moved to symbolTable because it changes with contex
extern int rfpOffset;
int rbssOffset = 0;

void printCode(struct node* topNode){
    struct code* code = topNode->code;
    while(code->previous){
        code = code->previous;
    }
    while(code){
        printf("%s\n",code->line);
        code = code->next; 
    }
}

void updateNodeCodeOPERATION(struct node* topNode, struct node* leftOperand, struct node* rightOperand, struct node* operatorNode){
    topNode->registerTemp = newRegister();
    topNode->code = newCode();

    char* leftRegisterFromIdentifier = "";
    char* rightRegisterFromIdentifier = "";
    if(leftOperand->token_type == NATUREZA_IDENTIFICADOR){
        struct symbolInfo* info = findSymbolInContexts(leftOperand->token_value);
        if(info != NULL){
            leftRegisterFromIdentifier = info->registerTemp;
        }
    }
    if(rightOperand->token_type == NATUREZA_IDENTIFICADOR){
        struct symbolInfo* info = findSymbolInContexts(rightOperand->token_value);
        if(info != NULL){
            rightRegisterFromIdentifier = info->registerTemp;
        }
    }

    if(strcmp(leftOperand->token_value, AST_LITERAL) == 0) {
        if(strcmp(rightOperand->token_value, AST_LITERAL) == 0) {
            //both literals
            leftOperand->registerTemp = newRegister(); //TODO LIBERAR ESSE REG
            //load to register
            struct code* previousCode = newCode();
            topNode->code->previous = previousCode;
            previousCode->next = topNode->code;
            
            strcat(previousCode->line,"loadI ");
            strcat(previousCode->line,calculateCodeGenValue(leftOperand->child));
            strcat(previousCode->line," => ");
            strcat(previousCode->line,leftOperand->registerTemp);

            //add imediate   
            if(strcmp(operatorNode->token_value, "+") == 0) {
                strcat(topNode->code->line,"addI ");
            }
            else if(strcmp(operatorNode->token_value, "-") == 0){ //minus
                strcat(topNode->code->line,"subI ");
            }
            else if(strcmp(operatorNode->token_value, "*") == 0){ //multiplication
                strcat(topNode->code->line,"multI ");
            }
            else{ //division
                strcat(topNode->code->line,"divI ");
            }

            strcat(topNode->code->line,leftOperand->registerTemp);
            strcat(topNode->code->line,", ");
            strcat(topNode->code->line,calculateCodeGenValue(rightOperand->child));
            strcat(topNode->code->line," => ");
            strcat(topNode->code->line,topNode->registerTemp);

        }
        else{ //left literal right register

            //concat code
            topNode->code->previous = rightOperand->code;
            rightOperand->code->next = topNode->code;

            //add imediate   
            if(strcmp(operatorNode->token_value, "+") == 0) {
                strcat(topNode->code->line,"addI ");
            }
            else if(strcmp(operatorNode->token_value, "-") == 0){ //minus
                strcat(topNode->code->line,"subI ");
            }
            else if(strcmp(operatorNode->token_value, "*") == 0){ //multiplication
                strcat(topNode->code->line,"multI ");
            }
            else{ //division
                strcat(topNode->code->line,"divI ");
            }

            char* attributionRegister;
            if(rightOperand->token_type != NATUREZA_IDENTIFICADOR)
                attributionRegister = rightOperand->registerTemp;
            else
                attributionRegister = rightRegisterFromIdentifier;

            strcat(topNode->code->line,attributionRegister);
            strcat(topNode->code->line,",");
            strcat(topNode->code->line,calculateCodeGenValue(leftOperand->child));
            strcat(topNode->code->line,"=>");
            strcat(topNode->code->line,topNode->registerTemp);
        }
    }

    else { //left is register
        if(strcmp(rightOperand->token_value, AST_LITERAL) == 0) { //right is literal
            //concat code
            topNode->code->previous = leftOperand->code;
            leftOperand->code->next = topNode->code;

            if(strcmp(operatorNode->token_value, "+") == 0) {
                strcat(topNode->code->line,"addI ");
            }
            else if(strcmp(operatorNode->token_value, "-") == 0){ //minus
                strcat(topNode->code->line,"subI ");
            }
            else if(strcmp(operatorNode->token_value, "*") == 0){ //multiplication
                strcat(topNode->code->line,"multI ");
            }
            else{ //division
                strcat(topNode->code->line,"divI ");
            }

            char* attributionRegister;
            if(leftOperand->token_type != NATUREZA_IDENTIFICADOR)
                attributionRegister = leftOperand->registerTemp;
            else
                attributionRegister = leftRegisterFromIdentifier;

            strcat(topNode->code->line,attributionRegister);
            strcat(topNode->code->line,", ");
            strcat(topNode->code->line,calculateCodeGenValue(rightOperand->child));
            strcat(topNode->code->line," => ");
            strcat(topNode->code->line,topNode->registerTemp);

        } 
        else { //both are registers
            //concat code
            if(leftOperand->code){
                topNode->code->previous = leftOperand->code;
                leftOperand->code->next = topNode->code;

                if(rightOperand->code){
                    leftOperand->code->previous = rightOperand->code;
                    rightOperand->code->next = leftOperand->code;

                }
            }
            if(rightOperand->code){
                topNode->code->previous = rightOperand->code;
                rightOperand->code->next = topNode->code;
            }
            // rightOperand->code->previous = leftOperand->code;
            // topNode->code->previous = rightOperand->code;

            //add imediate   
            if(strcmp(operatorNode->token_value, "+") == 0) {
                strcat(topNode->code->line,"add ");
            }
            else if(strcmp(operatorNode->token_value, "-") == 0){ //minus
                strcat(topNode->code->line,"sub ");
            }
            else if(strcmp(operatorNode->token_value, "*") == 0){ //multiplication
                strcat(topNode->code->line,"mult ");
            }
            else{ //division
                strcat(topNode->code->line,"div ");
            }

            char* leftAttributionRegister;
            char* rightAttributionRegister;
            if(leftOperand->token_type != NATUREZA_IDENTIFICADOR)
                leftAttributionRegister = leftOperand->registerTemp;
            else
                leftAttributionRegister = leftRegisterFromIdentifier;

            if(rightOperand->token_type != NATUREZA_IDENTIFICADOR)
                rightAttributionRegister = rightOperand->registerTemp;
            else
                rightAttributionRegister = rightRegisterFromIdentifier;

            strcat(topNode->code->line,leftAttributionRegister);
            strcat(topNode->code->line,", ");
            strcat(topNode->code->line,rightAttributionRegister);
            strcat(topNode->code->line," => ");
            strcat(topNode->code->line,topNode->registerTemp);

        }
    }
}

void updateNodeCodeLOCALDECLARATION(struct node* topNode, struct node* identifierNode, struct node* typeNode){
    struct symbolInfo* info = findSymbolInCurrentContext(identifierNode->token_value);

    if(info == NULL){
        info = findSymbolInContexts(identifierNode->token_value);
        if(info == NULL)
            return;
        else
            updateNodeCodeGLOBALDECLARATION(topNode, identifierNode, typeNode);
    }
    else {
        topNode->registerTemp = newRegister();
        topNode->code = newCode();

        int localRfpOffset = 0;

        //increment global rfp index 
        if(strcmp(typeNode->token_value, "bool") == 0){ //TODO tem algum jeito de fazer isso com type ou nature do nodo?
            rfpOffset += 1;
            localRfpOffset = 1;
        }
        else if(strcmp(typeNode->token_value, "int") == 0){
            rfpOffset += 4;
            localRfpOffset = 4;

        }
        else{
            printf("wrong type!\n");
        }
    
        //put in symbol table: regtemp, rfp
        info->registerTemp = topNode->registerTemp;
        info->rfpOffset = localRfpOffset;
        //increment rfp in the code 
        strcat(topNode->code->line,"addI ");
        strcat(topNode->code->line,"rfp, ");

        char offSetValue[2];
        sprintf(offSetValue,"%d", localRfpOffset);
        strcat(topNode->code->line, offSetValue);
        strcat(topNode->code->line," => ");
        strcat(topNode->code->line,"rfp");
    }
}

void updateNodeCodeGLOBALDECLARATION(struct node* topNode, struct node* identifierNode, struct node* typeNode){
    topNode->registerTemp = newRegister();
    topNode->code = newCode();
    int localRbssOffset = 0;

    //increment global rfp index 
    if(strcmp(typeNode->token_value, "bool") == 0){ //TODO tem algum jeito de fazer isso com type ou nature do nodo?
        rbssOffset += 1;
        localRbssOffset = 1;
    }
    else if(strcmp(typeNode->token_value, "int") == 0){
        rbssOffset += 4;
        localRbssOffset = 4;
    }
    else{
        printf("wrong type!\n");
    }


    struct symbolInfo* info = findSymbolInContexts(identifierNode->token_value);
    
    //put in symbol table: regtemp, rfp
    info->registerTemp = topNode->registerTemp;
    info->rbssOffset = localRbssOffset;

    //increment rfp in the code 
    strcat(topNode->code->line,"addI ");
    strcat(topNode->code->line,"rbss, ");
    char offSetValue[2];
    sprintf(offSetValue,"%d", localRbssOffset);
    strcat(topNode->code->line, offSetValue);
    strcat(topNode->code->line," => ");
    strcat(topNode->code->line,"rbss");
}

void updateNodeCodeATTRIBUTION(struct node* topNode, struct node* leftOperand, struct node* rightOperand){
    struct symbolInfo* info = findSymbolInContexts(leftOperand->token_value);

    char* attributionRegister = info->registerTemp;
    topNode->code = newCode();
    char* registerName = "";
    if(strcmp(rightOperand->token_value, AST_LITERAL) == 0){
        rightOperand->registerTemp = newRegister(); //TODO LIBERAR ESSE REG
        struct code* previousCode = newCode();
        topNode->code->previous = previousCode;
        previousCode->next = topNode->code;

        strcat(previousCode->line,"loadI ");
        strcat(previousCode->line,calculateCodeGenValue(rightOperand->child));
        strcat(previousCode->line," => ");
        strcat(previousCode->line,rightOperand->registerTemp);

        registerName = rightOperand->registerTemp;
    }
    else{
        registerName = rightOperand->registerTemp;
        topNode->code->previous = rightOperand->code;
        rightOperand->code->next = topNode->code;

    }
    strcat(topNode->code->line,"store ");
    strcat(topNode->code->line,registerName);
    strcat(topNode->code->line," => ");
    strcat(topNode->code->line,attributionRegister);

    topNode->registerTemp = leftOperand->registerTemp;
}

char* calculateCodeGenValue(struct node* node){
    if(node->token_type == NATUREZA_LITERAL_INT)
        return node->token_value;
    else if (node->token_type == NATUREZA_LITERAL_BOOL){
        if(strcmp(node->token_value, "true") == 0)
            return "1"; // true to int
        else
            return "0";
    }
    else if(node->token_type == NATUREZA_LITERAL_FLOAT){
        return "FLOAT";
    }
    return "";
}

char* newRegister(){
    char registerNumber[10];
    sprintf(registerNumber,"%d", registerIndex++);
    char* registerName = malloc(sizeof(char)*10);
    strcat(registerName, "r");
    strcat(registerName, registerNumber);
    strcat(registerName, "\0");

    return registerName;
}
char* newLabel(){
    char labelNumber[10];
    sprintf(labelNumber,"%d", labelIndex++);
    char* labelName = malloc(sizeof(char)*10);
    strcat(labelName, "r");
    strcat(labelName, labelNumber);

    return labelName;
}

struct code* newCode(){
    struct code* newCode = malloc(sizeof(struct code*));
    newCode->previous = NULL;
    newCode->line = malloc(sizeof(char)*100);//size of string is 100
    newCode->next = NULL;
    return newCode;
}

struct code* concatTwoCodes(struct node* executedFirst, struct node* executedSecond){
    struct code* code = NULL;
    struct code* codeIterator = NULL;

    if(executedFirst->code) {
        code = executedFirst->code;
        while(code->next){
            code = code->next;
        }
        if(executedSecond->code){
            codeIterator = executedSecond->code;
            while(codeIterator->previous){
                codeIterator = codeIterator->previous;
            }
            code->next = codeIterator;
            codeIterator->previous = code;

        }
    }
    else if(executedSecond->code){
        code = executedSecond->code;
    }

           
    return code;
}