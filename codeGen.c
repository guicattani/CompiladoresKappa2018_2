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

void updateNodeCodeARITHCOMPARISON(struct node* topNode, struct node* leftOperand, struct node* rightOperand, struct node* operatorNode){
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
            leftOperand->registerTemp = newRegister();  //liberar esses regs quando for liberar o código! TODO
            rightOperand->registerTemp = newRegister(); 
            //load to register left
            struct code* prePreviousCode = newCode();
            //load to register right
            struct code* previousCode = newCode();
            //load to register
            struct code* compareCode = newCode();

            previousCode->previous = prePreviousCode;
            prePreviousCode->next = previousCode;
            
            compareCode->previous = previousCode;
            previousCode->next = compareCode;

            topNode->code->previous = compareCode;
            compareCode->next = topNode->code;
            
            strcat(prePreviousCode->line,"loadI ");
            strcat(prePreviousCode->line,calculateCodeGenValue(leftOperand->child));
            strcat(prePreviousCode->line," => ");
            strcat(prePreviousCode->line,leftOperand->registerTemp);
            
            strcat(previousCode->line,"loadI ");
            strcat(previousCode->line,calculateCodeGenValue(rightOperand->child));
            strcat(previousCode->line," => ");
            strcat(previousCode->line,rightOperand->registerTemp);

            if(strcmp(operatorNode->token_value, "<") == 0) {
                strcat(compareCode->line,"cmp_LT ");
            }
            else if(strcmp(operatorNode->token_value, "<=") == 0){ //minus
                strcat(compareCode->line,"cmp_LE ");
            }
            else if(strcmp(operatorNode->token_value, "=") == 0){ //multiplication
                strcat(compareCode->line,"cmp_EQ ");
            }
            else if(strcmp(operatorNode->token_value, ">=") == 0){ //multiplication
                strcat(compareCode->line,"cmp_GE ");
            }
            else if(strcmp(operatorNode->token_value, ">") == 0){ //multiplication
                strcat(compareCode->line,"cmp_GT ");
            }
            else if(strcmp(operatorNode->token_value, "&&") == 0){ //multiplication
                strcat(compareCode->line,"and ");
            }
            else if(strcmp(operatorNode->token_value, "||") == 0){ //multiplication
                strcat(compareCode->line,"or ");
            }
            else{
                strcat(compareCode->line,"cmp_NE ");
            }

            strcat(compareCode->line,leftOperand->registerTemp);
            strcat(compareCode->line,", ");
            strcat(compareCode->line,rightOperand->registerTemp);
            strcat(compareCode->line," -> ");
            strcat(compareCode->line,topNode->registerTemp);

            strcat(topNode->code->line,"cbr ");
            strcat(topNode->code->line,topNode->registerTemp);
            strcat(topNode->code->line," -> ");
            strcat(topNode->code->line,"#1,");
            strcat(topNode->code->line,"#2");

        }
        else{ //left literal right register

            //concat code
            struct code* compareCode = newCode();
            struct code* previousCode = newCode();
            if(leftOperand->token_type != NATUREZA_IDENTIFICADOR) {
                previousCode = leftOperand->code;
                previousCode->next = compareCode;
                
                compareCode->previous = previousCode;
                compareCode->next = topNode->code;

                topNode->code->previous = compareCode;
            }
            else{
                free(previousCode);

                topNode->code->previous = compareCode;
                compareCode->next = topNode->code;
            }

            leftOperand->registerTemp = newRegister(); 

            struct code* registerLoad = newCode();
            if(compareCode->previous)
                registerLoad->previous = compareCode->previous;
            compareCode->previous = registerLoad;
            registerLoad->next = compareCode;

            leftOperand->registerTemp = newRegister(); 

            strcat(registerLoad->line,"loadI ");
            strcat(registerLoad->line,calculateCodeGenValue(leftOperand->child));
            strcat(registerLoad->line," => ");
            strcat(registerLoad->line,leftOperand->registerTemp);
            
            if(strcmp(operatorNode->token_value, "<") == 0) {
                strcat(compareCode->line,"cmp_LT ");
            }
            else if(strcmp(operatorNode->token_value, "<=") == 0){ //minus
                strcat(compareCode->line,"cmp_LE ");
            }
            else if(strcmp(operatorNode->token_value, "=") == 0){ //multiplication
                strcat(compareCode->line,"cmp_EQ ");
            }
            else if(strcmp(operatorNode->token_value, ">=") == 0){ //multiplication
                strcat(compareCode->line,"cmp_GE ");
            }
            else if(strcmp(operatorNode->token_value, ">") == 0){ //multiplication
                strcat(compareCode->line,"cmp_GT ");
            }
            else if(strcmp(operatorNode->token_value, "&&") == 0){ //multiplication
                strcat(compareCode->line,"and ");
            }
            else if(strcmp(operatorNode->token_value, "||") == 0){ //multiplication
                strcat(compareCode->line,"or ");
            }
            else{
                strcat(compareCode->line,"cmp_NE ");
            }

            char* attributionRegister;
            if(rightOperand->token_type != NATUREZA_IDENTIFICADOR)
                attributionRegister = rightOperand->registerTemp;
            else
                attributionRegister = rightRegisterFromIdentifier;

            strcat(compareCode->line,attributionRegister);
            strcat(compareCode->line,",");
            strcat(compareCode->line,leftOperand->registerTemp);
            strcat(compareCode->line,"->");
            strcat(compareCode->line,topNode->registerTemp);

            strcat(topNode->code->line,"cbr ");
            strcat(topNode->code->line,topNode->registerTemp);
            strcat(topNode->code->line," -> ");
            strcat(topNode->code->line,"#1,");
            strcat(topNode->code->line,"#2");
        }
    }

    else { //left is register
        if(strcmp(rightOperand->token_value, AST_LITERAL) == 0) { //right is literal
            //concat code
            struct code* compareCode = newCode();
            struct code* previousCode = newCode();
            if(leftOperand->token_type != NATUREZA_IDENTIFICADOR) {
                previousCode = leftOperand->code;
                previousCode->next = compareCode;
                
                compareCode->previous = previousCode;
                compareCode->next = topNode->code;

                topNode->code->previous = compareCode;
            }
            else{
                free(previousCode);

                topNode->code->previous = compareCode;
                compareCode->next = topNode->code;
            }

            struct code* registerLoad = newCode();
            if(compareCode->previous)
                registerLoad->previous = compareCode->previous;
            compareCode->previous = registerLoad;
            registerLoad->next = compareCode;
            rightOperand->registerTemp = newRegister(); 

            strcat(registerLoad->line,"loadI ");
            strcat(registerLoad->line,calculateCodeGenValue(rightOperand->child));
            strcat(registerLoad->line," => ");
            strcat(registerLoad->line,rightOperand->registerTemp);

            if(strcmp(operatorNode->token_value, "<") == 0) {
                strcat(compareCode->line,"cmp_LT ");
            }
            else if(strcmp(operatorNode->token_value, "<=") == 0){ //minus
                strcat(compareCode->line,"cmp_LE ");
            }
            else if(strcmp(operatorNode->token_value, "=") == 0){ //multiplication
                strcat(compareCode->line,"cmp_EQ ");
            }
            else if(strcmp(operatorNode->token_value, ">=") == 0){ //multiplication
                strcat(compareCode->line,"cmp_GE ");
            }
            else if(strcmp(operatorNode->token_value, ">") == 0){ //multiplication
                strcat(compareCode->line,"cmp_GT ");
            }
            else if(strcmp(operatorNode->token_value, "&&") == 0){ //multiplication
                strcat(compareCode->line,"and ");
            }
            else if(strcmp(operatorNode->token_value, "||") == 0){ //multiplication
                strcat(compareCode->line,"or ");
            }
            else{
                strcat(compareCode->line,"cmp_NE ");
            }

            char* attributionRegister;
            if(leftOperand->token_type != NATUREZA_IDENTIFICADOR)
                attributionRegister = leftOperand->registerTemp;
            else
                attributionRegister = leftRegisterFromIdentifier;

            strcat(compareCode->line,attributionRegister);
            strcat(compareCode->line,", ");
            strcat(compareCode->line,calculateCodeGenValue(rightOperand->child));
            strcat(compareCode->line," => ");
            strcat(compareCode->line,topNode->registerTemp);

            strcat(topNode->code->line,"cbr ");
            strcat(topNode->code->line,topNode->registerTemp);
            strcat(topNode->code->line," -> ");
            strcat(topNode->code->line,"#1,");
            strcat(topNode->code->line,"#2");

        } 
        else { //both are registers
            //concat code
            if(strcmp(operatorNode->token_value, "&&") == 0){

            
                char* temp = newLabel();

                patching(leftOperand->code, temp, 1);
                printf("To na exp\n");
                concatTwoCodes(topNode, leftOperand);

                struct code* nextLine = getNextLine(topNode->code);
                strcat(nextLine->line, temp);
                strcat(nextLine->line, ":");
                            

                concatTwoCodes(topNode, rightOperand);

                struct code* compareCode = newCode();
                struct code* previousCode = newCode();
                if(rightOperand->token_type != NATUREZA_IDENTIFICADOR) {
                    previousCode->previous = rightOperand->code;
                    compareCode->next = previousCode;
                }
                else{
                    free(previousCode);
                }

                strcat(compareCode->line, "and ");


                char* leftAttributionRegister;
                char* rightAttributionRegister;
                if(leftOperand->token_type != NATUREZA_IDENTIFICADOR){
                    leftAttributionRegister = leftOperand->registerTemp;
                }
                else
                    leftAttributionRegister = leftRegisterFromIdentifier;

                if(rightOperand->token_type != NATUREZA_IDENTIFICADOR){
                    rightAttributionRegister = rightOperand->registerTemp;

                }
                else
                    rightAttributionRegister = rightRegisterFromIdentifier;

                strcat(compareCode->line,leftAttributionRegister);
                strcat(compareCode->line,", ");
                strcat(compareCode->line,rightAttributionRegister);
                strcat(compareCode->line," -> ");
                strcat(compareCode->line,topNode->registerTemp);

                strcat(topNode->code->line,"cbr ");
                strcat(topNode->code->line,topNode->registerTemp);
                strcat(topNode->code->line," -> ");
                strcat(topNode->code->line,"#1,");
                strcat(topNode->code->line,"#2");    

                

            }
            else{


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

                //concat code
                struct code* compareCode = newCode();
                struct code* previousCode = newCode();
                if(rightOperand->token_type != NATUREZA_IDENTIFICADOR) {
                    previousCode->previous = rightOperand->code;
                    compareCode->next = previousCode;
                }
                else{
                    free(previousCode);
                }

                if(strcmp(operatorNode->token_value, "<") == 0) {
                    strcat(compareCode->line,"cmp_LT ");
                }
                else if(strcmp(operatorNode->token_value, "<=") == 0){ //minus
                    strcat(compareCode->line,"cmp_LE ");
                }
                else if(strcmp(operatorNode->token_value, "=") == 0){ //multiplication
                    strcat(compareCode->line,"cmp_EQ ");
                }
                else if(strcmp(operatorNode->token_value, ">=") == 0){ //multiplication
                    strcat(compareCode->line,"cmp_GE ");
                }
                else if(strcmp(operatorNode->token_value, ">") == 0){ //multiplication
                    strcat(compareCode->line,"cmp_GT ");
                }
                else{
                    strcat(compareCode->line,"cmp_NE ");
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

                strcat(compareCode->line,leftAttributionRegister);
                strcat(compareCode->line,", ");
                strcat(compareCode->line,rightAttributionRegister);
                strcat(compareCode->line," -> ");
                strcat(compareCode->line,topNode->registerTemp);

                strcat(topNode->code->line,"cbr ");
                strcat(topNode->code->line,topNode->registerTemp);
                strcat(topNode->code->line," -> ");
                strcat(topNode->code->line,"#1,");
                strcat(topNode->code->line,"#2");
            }

        }
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
            if(rightOperand->token_type != NATUREZA_IDENTIFICADOR) {
                topNode->code->previous = rightOperand->code;
                rightOperand->code->next = topNode->code;
            }

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
            if(leftOperand->token_type != NATUREZA_IDENTIFICADOR) {
                
                topNode->code->previous = leftOperand->code;
                leftOperand->code->next = topNode->code;
            }

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
            printf("cu");

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
        topNode->registerTemp = newRegister(); //for the sake of completion
        topNode->code = newCode();

        //no code necessary, because ins't initialized

        //put in symbol table: regtemp, rfp
        info->rfpOffset = rfpOffset;
        rfpOffset += 4;
    
        //put in symbol table: regtemp, rfp
        info->registerTemp = topNode->registerTemp;
    }
}

void updateNodeCodeGLOBALDECLARATION(struct node* topNode, struct node* identifierNode, struct node* typeNode){
    topNode->registerTemp = newRegister();
    topNode->code = newCode();

    //Load registers
    char* initializationRegisterLoad = newRegister(); //esse fica dangling, tem que dar free TODO
    struct code* previousCodeLoad = newCode();
    topNode->code->previous = previousCodeLoad;
    previousCodeLoad->next = topNode->code;
        
    strcat(previousCodeLoad->line,"loadI ");
    strcat(previousCodeLoad->line,"0");
    strcat(previousCodeLoad->line," => ");
    strcat(previousCodeLoad->line,initializationRegisterLoad);
        
    strcat(topNode->code->line,"storeAI ");
    strcat(topNode->code->line,initializationRegisterLoad);
    strcat(topNode->code->line," => ");
    strcat(topNode->code->line,"rbss");
    strcat(topNode->code->line,", ");
    char rbssOffsetTemp[10];
    sprintf(rbssOffsetTemp,"%d", rbssOffset);
    strcat(topNode->code->line,rbssOffsetTemp);

    struct symbolInfo* info = findSymbolInContexts(identifierNode->token_value);
    info->rbssOffset = rbssOffset;

    //increment global rbss index 
    rbssOffset += 4;

    //put in symbol table: regtemp, rfp
    info->registerTemp = topNode->registerTemp;
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

    strcat(topNode->code->line,"storeAI ");
    strcat(topNode->code->line,rightOperand->registerTemp);
    strcat(topNode->code->line," => ");
    if(info->rfpOffset >= 0){
        strcat(topNode->code->line,"rfp");
        strcat(topNode->code->line,", ");
        char rfpOffsetTemp[10];
        sprintf(rfpOffsetTemp,"%d", info->rfpOffset);
        strcat(topNode->code->line,rfpOffsetTemp);
    }
    else{
        strcat(topNode->code->line,"rbss");
        strcat(topNode->code->line,", ");
        char rbssOffsetTemp[10];
        sprintf(rbssOffsetTemp,"%d", info->rbssOffset);
        strcat(topNode->code->line,rbssOffsetTemp);
    }
    topNode->registerTemp = "test";

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
    registerName[0] = '\0';
    strcat(registerName, "r");
    strcat(registerName, registerNumber);
    strcat(registerName, "\0");
    return registerName;
}
char* newLabel(){
    char labelNumber[10];
    sprintf(labelNumber,"%d", labelIndex++);
    char* labelName = malloc(sizeof(char)*10);
    labelName[0] = '\0';
    strcat(labelName, "r");
    strcat(labelName, labelNumber);

    return labelName;
}

struct code* newCode(){
    struct code* newCode = malloc(sizeof(struct code*));
    newCode->previous = NULL;
    newCode->line = malloc(sizeof(char)*100);//size of string is 100
    newCode->line[0] = '\0';
    newCode->next = NULL;
    return newCode;
}

struct code* concatTwoCodes(struct node* executedFirst, struct node* executedSecond){
    struct code* codeIterator = NULL;
    struct code* returnCode = NULL;

    if(executedFirst->code) {
        returnCode = executedFirst->code;
        if(executedSecond->code){
            codeIterator = executedSecond->code;
            while(codeIterator->previous){
                codeIterator = codeIterator->previous;
            }
            returnCode->next = codeIterator;
            codeIterator->previous = returnCode;

            codeIterator = returnCode->next;
            while(codeIterator->next){
                codeIterator = codeIterator->next;
            }
            returnCode = codeIterator;
        }
    }
    else if(executedSecond->code){

        codeIterator = executedSecond->code;
        while(codeIterator->next){
            codeIterator = codeIterator->next;
        }
        returnCode = codeIterator;
    }
           
    return returnCode;
}

void updateNodeCodeIF(struct node* ifNode, struct node* condition, struct node* ifTrue){
    ifNode->code = newCode();
    char * rot = newRegister();
    char * rot2 = newRegister();
    patching(condition->code, rot, 1);
    patching(condition->code, rot2, 0);

    printf("oi\n");

    ifNode->code = concatTwoCodes(ifNode, condition);


    //nextline should have the last line of code
    struct code* nextLine = getNextLine(ifNode->code);
    
    strcat(nextLine->line, rot);
    strcat(nextLine->line, ":");
    
    ifNode->code = concatTwoCodes(ifNode, ifTrue);


    //nextline should have the last line of code
    nextLine = getNextLine(ifNode->code);
    

    strcat(nextLine->line, rot2);
    strcat(nextLine->line, ":");

    
    free(rot);
    free(rot2);

}

void updateNodeCodeIFELSE(struct node* ifNode, struct node* condition, struct node* ifTrue, struct node* ifFalse){
    ifNode->code = newCode();
    char * rot = newLabel();
    char * rot2 = newLabel();
    char * end = newLabel();
    patching(condition->code, rot, 1);
    patching(condition->code, rot2, 0);

    ifNode->code = concatTwoCodes(ifNode, condition);
    
    //nextline should have the last line of code
    struct code* nextLine = getNextLine(ifNode->code);


    strcat(nextLine->line, rot);
    strcat(nextLine->line, ":");
    
    ifNode->code = concatTwoCodes(ifNode, ifTrue);


    //nextline should have the last line of code
    nextLine = getNextLine(ifNode->code);

    
    strcat(nextLine->line, "jumpI => ");
    strcat(nextLine->line, end);


    nextLine->next = newCode();
    nextLine->next->previous = nextLine;
    nextLine = nextLine->next;  


    strcat(nextLine->line, rot2);
    strcat(nextLine->line, ":");

    ifNode->code = concatTwoCodes(ifNode, ifFalse);

    nextLine = getNextLine(ifNode->code);

    strcat(nextLine->line, end);
    strcat(nextLine->line, ":");


    free(end);
    free(rot);
    free(rot2);




}


void updateCodeWHILE(struct node* whileNode, struct node* condition, struct node* commandsBlock){
    char* begin = newLabel();
    char* Btrue = newLabel();
    char* Bfalse = newLabel();

    patching(condition->code, Btrue, 1);
    patching(condition->code, Bfalse, 0);

    whileNode->code = newCode();
    strcat(whileNode->code->line, begin);
    strcat(whileNode->code->line, ":");

    whileNode->code = concatTwoCodes(whileNode, condition);

    struct code* NextLine = getNextLine(whileNode->code);
    strcat(NextLine->line, Btrue);
    strcat(NextLine->line, ":");

    whileNode->code = concatTwoCodes(whileNode, commandsBlock);
    
    NextLine = getNextLine(whileNode->code);
    strcat(NextLine->line, "jumpI =>");
    strcat(NextLine->line, begin);

    NextLine = getNextLine(whileNode->code);
    strcat(NextLine->line, Bfalse);
    strcat(NextLine->line, ":");   



}

void updateCodeDOWHILE(struct node* whileNode, struct node* condition, struct node* commandsBlock){

    char* Btrue = newLabel();
    char* Bfalse = newLabel();
    patching(condition->code, Btrue, 1);
    patching(condition->code, Bfalse, 0);


    whileNode->code = newCode();
    strcat(whileNode->code->line, Btrue);
    strcat(whileNode->code->line, ":");

    whileNode->code = concatTwoCodes(whileNode, commandsBlock);

    whileNode->code = concatTwoCodes(whileNode, condition);
    

    struct code* NextLine = getNextLine(whileNode->code);
    strcat(NextLine->line, Bfalse);
    strcat(NextLine->line, ":");   



}

//Creates an empty code line at the end of code and returns it
struct code* getNextLine(struct code* code){

    if(code != NULL)
        while(code->next != NULL){
            code = code->next;
        }

    code->next = newCode();
    code->next->previous = code;
    code = code->next;

    return code;

}

//Patches the code with the string given
//par: Parameter - 1 replaces #1 with the replacement, 0 replaces #2
void patching(struct code* code, char* replacement, int par){

    while(code != NULL){
        printf("%s \n", code->line);
        fixLine(code->line, par, replacement);
        code = code->next;
        printf("%s \n", code->line);
    }
}

struct code* removeCBR(struct code* code){
    struct code* codeIterator = code;
    struct code* returnCode = code;

    //if first line is cbr
    char *token = strstr(codeIterator->line, "cbr");
    if(token){
        returnCode = code->previous;
    }

    while(codeIterator != NULL){
        token = strstr(codeIterator->line, "cbr");
        struct code* replaceCode = codeIterator->next;

        if(token){
            codeIterator->previous = codeIterator->next;
            free(codeIterator);
        }
        codeIterator = replaceCode;
    }

    return returnCode;
}

//Given a string to replace, searches for #1 or #2, and replaces it with the string given as argument
//Line: Line to be changed
//par: Parameter - 1 replaces #1 with the replacement, 0 replaces #2
void fixLine(char* line, int par, char* replacement){
    //Str should hold the string to be replaced
    char str[3] = "#1";
    if(!par)
        str[1] = '2';

    char* token, *dup;
    //Token will be a pointer to the place where we will need to start replacing
    token = strstr(line, str);
    if(token){
        printf("Token\n");
        //Dup is a temporary copy of the rest of the string so we can replace safely
        dup = strdup(token + 2);

        //Now we need to replace, then put the dup back on the string
        strcpy(token, replacement);
        //Thank god for \0 marking the end of strings
        strcat(token, dup);
    
        free(dup);
    }



}