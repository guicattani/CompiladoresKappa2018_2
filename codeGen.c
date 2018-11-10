#include "codeGen.h"

int registerIndex = 0;
int labelIndex = 0;

int rbssOffset = 0;
int rfpOffset = 0;

void printCode(struct node* topNode){
    struct code* code = topNode->code;
    while(code){
        printf("%s\n",code->line);
        code = code->previous; 
    }
}

void updateNodeCodeOPERATION(struct node* topNode, struct node* leftOperand, struct node* rightOperand, struct node* operatorNode){
    topNode->registerTemp = newRegister();
    topNode->code = newCode();
    
    if(strcmp(leftOperand->token_value, AST_LITERAL) == 0) {
        if(strcmp(rightOperand->token_value, AST_LITERAL) == 0) {
            //both literals
            leftOperand->registerTemp = newRegister(); //TODO LIBERAR ESSE REG
            //load to register
            topNode->code->previous = newCode();
            
            strcat(topNode->code->previous->line,"loadI ");
            strcat(topNode->code->previous->line,calculateCodeGenValue(leftOperand->child));
            strcat(topNode->code->previous->line," => ");
            strcat(topNode->code->previous->line,leftOperand->registerTemp);

                //add imediate   
            if(strcmp(operatorNode->token_value, "+") == 0) {
                strcat(topNode->code->line,"addI ");
            }
            else { //minus
                strcat(topNode->code->line,"subI ");
            }

            strcat(topNode->code->line,leftOperand->registerTemp);
            strcat(topNode->code->line,", ");
            strcat(topNode->code->line,calculateCodeGenValue(rightOperand->child));
            strcat(topNode->code->line," => ");
            strcat(topNode->code->line,topNode->registerTemp);

        }
        else { //left literal right register

            //concat code
            topNode->code->previous = rightOperand->code;

            if(strcmp(operatorNode->token_value, "+") == 0) {
                strcat(topNode->code->line,"addI");
            }
            else { //minus
                strcat(topNode->code->line,"subI");
            }
            strcat(topNode->code->line,rightOperand->registerTemp);
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

            if(strcmp(operatorNode->token_value, "+") == 0) {
                strcat(topNode->code->line,"addI ");
            }
            else { //minus
                strcat(topNode->code->line,"subI ");
            }
            strcat(topNode->code->line,leftOperand->registerTemp);
            strcat(topNode->code->line,", ");
            strcat(topNode->code->line,calculateCodeGenValue(rightOperand->child));
            strcat(topNode->code->line," => ");
            strcat(topNode->code->line,topNode->registerTemp);

        } 
        else { //both are registers
            //concat code
            rightOperand->code->previous = leftOperand->code;
            topNode->code->previous = rightOperand->code;

            if(strcmp(operatorNode->token_value, "+") == 0) {
                strcat(topNode->code->line,"add ");
            }
            else { //minus
                strcat(topNode->code->line,"sub ");
            }
            strcat(topNode->code->line,leftOperand->registerTemp);
            strcat(topNode->code->line,", ");
            strcat(topNode->code->line,rightOperand->registerTemp);
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

        int localRfpOffset = rfpOffset;

        //increment global rfp index 
        if(strcmp(typeNode->token_value, "bool") == 0){ //TODO tem algum jeito de fazer isso com type ou nature do nodo?
            rfpOffset += 1;
        }
        else if(strcmp(typeNode->token_value, "int") == 0){
            rfpOffset += 4;
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
        sprintf(offSetValue,"%d", rfpOffset);
        strcat(topNode->code->line, offSetValue);
        strcat(topNode->code->line," => ");
        strcat(topNode->code->line,"rfp");
    }
}

void updateNodeCodeGLOBALDECLARATION(struct node* topNode, struct node* identifierNode, struct node* typeNode){
    topNode->code = newCode();
    int localRbssOffset = rbssOffset;

    //increment global rfp index 
    if(strcmp(typeNode->token_value, "bool") == 0){ //TODO tem algum jeito de fazer isso com type ou nature do nodo?
        rbssOffset += 1;
    }
    else if(strcmp(typeNode->token_value, "int") == 0){
        rbssOffset += 4;
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
    leftOperand->registerTemp = newRegister(); //TODO LIBERAR ESSE REG
    topNode->code = newCode();
    topNode->code->previous = rightOperand->code;

    strcat(topNode->code->line,"store ");
    strcat(topNode->code->line,rightOperand->registerTemp);
    strcat(topNode->code->line," => ");
    strcat(topNode->code->line,leftOperand->registerTemp);

    topNode->registerTemp = leftOperand->registerTemp;

    printCode(topNode);
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

    if(executedFirst->code != NULL)
        code = executedFirst->code;
    if(executedSecond->code != NULL)
        code->previous = executedSecond->code;

    return code;
}