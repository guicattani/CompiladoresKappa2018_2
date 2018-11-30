#include "codeGen.h"

int registerIndex = 0;
int labelIndex = 0;
struct functionLabels *functionLabels = NULL;

//rfpOffset have been moved to symbolTable because it changes with contex
extern int rfpOffset;
int rbssOffset = 0;
int rspOffset = 0;
int dynamicLink = 0;
char* mainLabel = NULL;

void printCode(struct node* topNode){
    struct code* code = topNode->code;
    struct code* temp;
    while(code){
        printf("%s\n",code->line);
        temp = code;
        code = code->next; 
        free(temp->line);
        free(temp);
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
            if(info->type == NATUREZA_FUNC){

                topNode->code = concatTwoCodes(topNode->code, leftOperand->code);
                leftRegisterFromIdentifier = info->registerTemp;
            }
            else
                leftRegisterFromIdentifier = info->registerTemp;
        }
    }
    if(rightOperand->token_type == NATUREZA_IDENTIFICADOR){
        struct symbolInfo* info = findSymbolInContexts(rightOperand->token_value);
        if(info != NULL){
          if(info->type == NATUREZA_FUNC){
                topNode->code = concatTwoCodes(topNode->code, rightOperand->code);
                rightRegisterFromIdentifier = info->registerTemp;
            }
            else
                rightRegisterFromIdentifier= info->registerTemp;
        }
    }

    if(strcmp(leftOperand->token_value, AST_LITERAL) == 0) {
        if(strcmp(rightOperand->token_value, AST_LITERAL) == 0) {

            //both literals
            leftOperand->registerTemp = newRegister(); 
            rightOperand->registerTemp = newRegister(); 
            //load to register left
            struct code* next;
            
            strcat(topNode->code->line,"loadI ");
            strcat(topNode->code->line,calculateCodeGenValue(leftOperand->child));
            strcat(topNode->code->line," => ");
            strcat(topNode->code->line,leftOperand->registerTemp);
            
            next = getNextLine(topNode->code);

            strcat(next->line,"loadI ");
            strcat(next->line,calculateCodeGenValue(rightOperand->child));
            strcat(next->line," => ");
            strcat(next->line,rightOperand->registerTemp);

            next = getNextLine(topNode->code);

            if(strcmp(operatorNode->token_value, "<") == 0) {
                strcat(next->line,"cmp_LT ");
            }
            else if(strcmp(operatorNode->token_value, "<=") == 0){ //minus
                strcat(next->line,"cmp_LE ");
            }
            else if(strcmp(operatorNode->token_value, "=") == 0){ //multiplication
                strcat(next->line,"cmp_EQ ");
            }
            else if(strcmp(operatorNode->token_value, ">=") == 0){ //multiplication
                strcat(next->line,"cmp_GE ");
            }
            else if(strcmp(operatorNode->token_value, ">") == 0){ //multiplication
                strcat(next->line,"cmp_GT ");
            }
            else if(strcmp(operatorNode->token_value, "&&") == 0){ //multiplication
                strcat(next->line,"and ");
            }
            else if(strcmp(operatorNode->token_value, "||") == 0){ //multiplication
                strcat(next->line,"or ");
            }
            else{
                strcat(next->line,"cmp_NE ");
            }

            strcat(next->line,leftOperand->registerTemp);
            strcat(next->line,", ");
            strcat(next->line,rightOperand->registerTemp);
            strcat(next->line," -> ");
            strcat(next->line,topNode->registerTemp);

            next = getNextLine(topNode->code);

            strcat(next->line,"cbr ");
            strcat(next->line,topNode->registerTemp);
            strcat(next->line," -> ");
            strcat(next->line,"#1,");
            strcat(next->line,"#2");

        }
        else{ //left literal right register
            struct code* next;

            topNode->code = concatTwoCodes(leftOperand->code, topNode->code);
            leftOperand->registerTemp = newRegister(); 
            next = getNextLine(topNode->code);
            strcat(next->line,"loadI ");
            strcat(next->line,calculateCodeGenValue(leftOperand->child));
            strcat(next->line," => ");
            strcat(next->line,leftOperand->registerTemp);
            
            next = getNextLine(topNode->code);

            if(strcmp(operatorNode->token_value, "<") == 0) {
                strcat(next->line,"cmp_LT ");
            }
            else if(strcmp(operatorNode->token_value, "<=") == 0){ //minus
                strcat(next->line,"cmp_LE ");
            }
            else if(strcmp(operatorNode->token_value, "=") == 0){ //multiplication
                strcat(next->line,"cmp_EQ ");
            }
            else if(strcmp(operatorNode->token_value, ">=") == 0){ //multiplication
                strcat(next->line,"cmp_GE ");
            }
            else if(strcmp(operatorNode->token_value, ">") == 0){ //multiplication
                strcat(next->line,"cmp_GT ");
            }
            else if(strcmp(operatorNode->token_value, "&&") == 0){ //multiplication
                strcat(next->line,"and ");
            }
            else if(strcmp(operatorNode->token_value, "||") == 0){ //multiplication
                strcat(next->line,"or ");
            }
            else{
                strcat(next->line,"cmp_NE ");
            }

            char* attributionRegister;


            if(rightOperand->token_type != NATUREZA_IDENTIFICADOR)
                attributionRegister = rightOperand->registerTemp;
            else
                attributionRegister = rightRegisterFromIdentifier;

            strcat(next->line,attributionRegister);
            strcat(next->line,",");
            strcat(next->line,leftOperand->registerTemp);
            strcat(next->line,"->");
            strcat(next->line,topNode->registerTemp);

            next = getNextLine(topNode->code);

            strcat(next->line,"cbr ");
            strcat(next->line,topNode->registerTemp);
            strcat(next->line," -> ");
            strcat(next->line,"#1,");
            strcat(next->line,"#2");
        }
    }

    else { //left is not literal
        if(strcmp(rightOperand->token_value, AST_LITERAL) == 0) { //right is literal
            //concat code
            struct code* next;
            topNode->code = concatTwoCodes(leftOperand->code,topNode->code);
            rightOperand->registerTemp = newRegister(); 

            next = getNextLine(topNode->code);
            strcat(next->line,"loadI ");
            strcat(next->line,calculateCodeGenValue(rightOperand->child));
            strcat(next->line," => ");
            strcat(next->line,rightOperand->registerTemp);
            next = getNextLine(topNode->code);

            if(strcmp(operatorNode->token_value, "<") == 0) {
                strcat(next->line,"cmp_LT ");
            }
            else if(strcmp(operatorNode->token_value, "<=") == 0){ //minus
                strcat(next->line,"cmp_LE ");
            }
            else if(strcmp(operatorNode->token_value, "=") == 0){ //multiplication
                strcat(next->line,"cmp_EQ ");
            }
            else if(strcmp(operatorNode->token_value, ">=") == 0){ //multiplication
                strcat(next->line,"cmp_GE ");
            }
            else if(strcmp(operatorNode->token_value, ">") == 0){ //multiplication
                strcat(next->line,"cmp_GT ");
            }
            else if(strcmp(operatorNode->token_value, "&&") == 0){ //multiplication
                strcat(next->line,"and ");
            }
            else if(strcmp(operatorNode->token_value, "||") == 0){ //multiplication
                strcat(next->line,"or ");
            }
            else{
                strcat(next->line,"cmp_NE ");
            }

            char* attributionRegister;
            if(leftOperand->token_type != NATUREZA_IDENTIFICADOR)
                attributionRegister = leftOperand->registerTemp;
            else
                attributionRegister = leftRegisterFromIdentifier;

            strcat(next->line,attributionRegister);
            strcat(next->line,", ");
            strcat(next->line,rightOperand->registerTemp);
            strcat(next->line," => ");
            strcat(next->line,topNode->registerTemp);

            next = getNextLine(next);

            strcat(next->line,"cbr ");
            strcat(next->line,topNode->registerTemp);
            strcat(next->line," -> ");
            strcat(next->line,"#1,");
            strcat(next->line,"#2");

        } 
        else { //both are registers
            //concat code
            if(strcmp(operatorNode->token_value, "&&") == 0){
                char* temp = newLabel();
                patching(leftOperand->code, temp, 1);
                if(leftOperand->code){
                    topNode->code = concatTwoCodes(topNode->code, leftOperand->code);
                }
                struct code* nextLine = getNextLine(topNode->code);
                strcat(nextLine->line, temp);
                strcat(nextLine->line, ":");
                topNode->code = concatTwoCodes(topNode->code, rightOperand->code);
                free(temp);
            }
            else if(strcmp(operatorNode->token_value, "||") == 0){
                char* temp = newLabel();
                patching(leftOperand->code, temp, 0);
                if(leftOperand->code){
                    topNode->code = concatTwoCodes(topNode->code, leftOperand->code);
                }
                struct code* nextLine = getNextLine(topNode->code);
                strcat(nextLine->line, temp);
                strcat(nextLine->line, ":");  
                topNode->code = concatTwoCodes(topNode->code, rightOperand->code);
                free(temp);
            }
            else{

                //concat code
 		concatTwoCodes(topNode->code, leftOperand->code);
 		concatTwoCodes(topNode->code, rightOperand->code);		
                struct code* nextLine;
                
                nextLine = getNextLine(topNode->code);
                
                if(strcmp(operatorNode->token_value, "<") == 0) {
                    strcat(nextLine->line,"cmp_LT ");
                }
                else if(strcmp(operatorNode->token_value, "<=") == 0){ //minus
                    strcat(nextLine->line,"cmp_LE ");
                }
                else if(strcmp(operatorNode->token_value, "=") == 0){ //multiplication
                    strcat(nextLine->line,"cmp_EQ ");
                }
                else if(strcmp(operatorNode->token_value, ">=") == 0){ //multiplication
                    strcat(nextLine->line,"cmp_GE ");
                }
                else if(strcmp(operatorNode->token_value, ">") == 0){ //multiplication
                    strcat(nextLine->line,"cmp_GT ");
                }
                else{
                    strcat(nextLine->line,"cmp_NE ");
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

                strcat(nextLine->line,leftAttributionRegister);
                strcat(nextLine->line,", ");
                strcat(nextLine->line,rightAttributionRegister);
                strcat(nextLine->line," -> ");
                strcat(nextLine->line,topNode->registerTemp);
	
		nextLine = getNextLine(topNode->code);

                strcat(nextLine->line,"cbr ");
                strcat(nextLine->line,topNode->registerTemp);
                strcat(nextLine->line," -> ");
                strcat(nextLine->line,"#1,");
                strcat(nextLine->line,"#2");
                
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
            if(info->type == NATUREZA_FUNC){

                topNode->code = concatTwoCodes(topNode->code, leftOperand->code);
                leftRegisterFromIdentifier = info->registerTemp;
            }
            else
                leftRegisterFromIdentifier = info->registerTemp;
        }
    }
    if(rightOperand->token_type == NATUREZA_IDENTIFICADOR){
        struct symbolInfo* info = findSymbolInContexts(rightOperand->token_value);
        if(info != NULL){
          if(info->type == NATUREZA_FUNC){
                topNode->code = concatTwoCodes(topNode->code, rightOperand->code);
                rightRegisterFromIdentifier = info->registerTemp;
            }
            else
                rightRegisterFromIdentifier= info->registerTemp;
        }
    }

    if(strcmp(leftOperand->token_value, AST_LITERAL) == 0) {
        if(strcmp(rightOperand->token_value, AST_LITERAL) == 0) {
            //both literals
            leftOperand->registerTemp = newRegister(); 
            //load to register
            struct code* next;
            
            strcat(topNode->code->line,"loadI ");
            strcat(topNode->code->line,calculateCodeGenValue(leftOperand->child));
            strcat(topNode->code->line," => ");
            strcat(topNode->code->line,leftOperand->registerTemp);

            next = getNextLine(topNode->code);
            //add imediate   
            if(strcmp(operatorNode->token_value, "+") == 0) {
                strcat(next->line,"addI ");
            }
            else if(strcmp(operatorNode->token_value, "-") == 0){ //minus
                strcat(next->line,"subI ");
            }
            else if(strcmp(operatorNode->token_value, "*") == 0){ //multiplication
                strcat(next->line,"multI ");
            }
            else{ //division
                strcat(next->line,"divI ");
            }

            strcat(next->line,leftOperand->registerTemp);
            strcat(next->line,", ");
            strcat(next->line,calculateCodeGenValue(rightOperand->child));
            strcat(next->line," => ");
            strcat(next->line,topNode->registerTemp);


        }
        else{ //left literal right register
            //concat code
            concatTwoCodes(topNode->code, rightOperand->code);

	        struct code* next = getNextLine(topNode->code);
            //add imediate   
            if(strcmp(operatorNode->token_value, "+") == 0) {
                strcat(next->line,"addI ");
            }
            else if(strcmp(operatorNode->token_value, "-") == 0){ //minus
                strcat(next->line,"subI ");
            }
            else if(strcmp(operatorNode->token_value, "*") == 0){ //multiplication
                strcat(next->line,"multI ");
            }
            else{ //division
                strcat(next->line,"divI ");
            }

            char* attributionRegister;
            if(rightOperand->token_type != NATUREZA_IDENTIFICADOR){
                if(rightOperand->registerTemp)
                    attributionRegister = rightOperand->registerTemp;
                else
                    attributionRegister = "ERROR";
            }
            else
                attributionRegister = rightRegisterFromIdentifier;
            

            strcat(next->line,attributionRegister);
            strcat(next->line,",");
            strcat(next->line,calculateCodeGenValue(leftOperand->child));
            strcat(next->line,"=>");
            strcat(next->line,topNode->registerTemp);
        }
    }

    else { //left is register
        if(strcmp(rightOperand->token_value, AST_LITERAL) == 0) { //right is literal
            //concat code
            
            concatTwoCodes(topNode->code, leftOperand->code);
            
            struct code* next = getNextLine(topNode->code);

            if(strcmp(operatorNode->token_value, "+") == 0) {
                strcat(next->line,"addI ");
            }
            else if(strcmp(operatorNode->token_value, "-") == 0){ //minus
                strcat(next->line,"subI ");
            }
            else if(strcmp(operatorNode->token_value, "*") == 0){ //multiplication
                strcat(next->line,"multI ");
            }
            else{ //division
                strcat(next->line,"divI ");
            }

            char* attributionRegister;
            if(leftOperand->token_type != NATUREZA_IDENTIFICADOR){
                if(leftOperand->registerTemp)
                    attributionRegister = leftOperand->registerTemp;
                else
                    attributionRegister = "ERROR";
            }
            else
                attributionRegister = leftRegisterFromIdentifier;

            strcat(next->line,attributionRegister);
            strcat(next->line,", ");
            strcat(next->line,calculateCodeGenValue(rightOperand->child));
            strcat(next->line," => ");
            strcat(next->line,topNode->registerTemp);
           

        } 
        else { //both are registers

            //concat code
            concatTwoCodes(topNode->code, leftOperand->code);
            concatTwoCodes(topNode->code, rightOperand->code);

	    struct code* next = getNextLine(topNode->code);
            //add imediate   
            if(strcmp(operatorNode->token_value, "+") == 0) {
                strcat(next->line,"add ");
            }
            else if(strcmp(operatorNode->token_value, "-") == 0){ //minus
                strcat(next->line,"sub ");
            }
            else if(strcmp(operatorNode->token_value, "*") == 0){ //multiplication
                strcat(next->line,"mult ");
            }
            else{ //division
                strcat(next->line,"div ");
            }

            char* leftAttributionRegister;
            char* rightAttributionRegister;
            if(leftOperand->token_type != NATUREZA_IDENTIFICADOR){
                if(leftOperand->registerTemp)
                    leftAttributionRegister = leftOperand->registerTemp;
                else
                    leftAttributionRegister = "ERROR";
            }
            else
                leftAttributionRegister = leftRegisterFromIdentifier;

            if(rightOperand->token_type != NATUREZA_IDENTIFICADOR){
                if(rightOperand->registerTemp)
                    rightAttributionRegister = rightOperand->registerTemp;
                else
                    rightAttributionRegister = "ERROR";
            }
            else
                rightAttributionRegister = rightRegisterFromIdentifier;

            strcat(next->line,leftAttributionRegister);
            strcat(next->line,", ");
            strcat(next->line,rightAttributionRegister);
            strcat(next->line," => ");
            strcat(next->line,topNode->registerTemp);

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
    strcat(topNode->code->line,"loadI ");
    strcat(topNode->code->line,"0");
    strcat(topNode->code->line," => ");
    strcat(topNode->code->line,topNode->registerTemp);
        
    struct code* next = getNextLine(topNode->code);
    
    strcat(next->line,"storeAI ");
    strcat(next->line,topNode->registerTemp);
    strcat(next->line," => ");
    strcat(next->line,"rbss");
    strcat(next->line,", ");
    char rbssOffsetTemp[10];
    sprintf(rbssOffsetTemp,"%d", rbssOffset);
    strcat(next->line,rbssOffsetTemp);

    struct symbolInfo* info = findSymbolInContexts(identifierNode->token_value);
    info->rbssOffset = rbssOffset;

    //increment global rbss index 
    rbssOffset += 4;

    //put in symbol table: regtemp, rfp
    info->registerTemp = topNode->registerTemp;

}

struct code* updateNodeCodeATTRIBUTION(struct node* topNode, struct node* leftOperand, struct node* rightOperand){
    struct symbolInfo* info = findSymbolInContexts(leftOperand->token_value);
    char* attributionRegister = info->registerTemp;
    topNode->code = newCode();
    char* registerName = "";

    if(strcmp(rightOperand->token_value, AST_LITERAL) == 0){
        char* printReg = info->registerTemp;
        struct code* next;

        strcat(topNode->code->line,"loadI ");
        strcat(topNode->code->line,calculateCodeGenValue(rightOperand->child));
        strcat(topNode->code->line," => ");
        strcat(topNode->code->line,printReg);
        registerName = printReg;

        next = getNextLine(topNode->code);

        strcat(next->line,"storeAI ");
        strcat(next->line,registerName);
        strcat(next->line," => ");
        if(info->rfpOffset >= 0){
            strcat(next->line,"rfp");
            strcat(next->line,", ");
            char rfpOffsetTemp[10];
            sprintf(rfpOffsetTemp,"%d", info->rfpOffset);
            strcat(next->line,rfpOffsetTemp);
        }
        else{
            strcat(next->line,"rbss");
            strcat(next->line,", ");
            char rbssOffsetTemp[10];
            sprintf(rbssOffsetTemp,"%d", info->rbssOffset);
            strcat(next->line,rbssOffsetTemp);
        }

    }
    else{
        struct symbolInfo* rightInfo;
        if(rightOperand->token_type == NATUREZA_IDENTIFICADOR){
            rightInfo = findSymbolInContexts(rightOperand->token_value);
            if(rightInfo->registerTemp)
                registerName = rightInfo->registerTemp;
            else
                registerName = "ERROR";
        }
        else{
            if(rightOperand->registerTemp)
                registerName = rightOperand->registerTemp;
            else
                registerName = "ERROR";
            topNode->code =  concatTwoCodes(rightOperand->code, topNode->code);
        }
        struct code* next = getNextLine(topNode->code);
        strcat(next->line,"storeAI ");
        strcat(next->line,registerName);
        strcat(next->line," => ");
        if(info->rfpOffset >= 0){
            strcat(next->line,"rfp");
            strcat(next->line,", ");
            char rfpOffsetTemp[10];
            sprintf(rfpOffsetTemp,"%d", info->rfpOffset);
            strcat(next->line,rfpOffsetTemp);
        }
        else{
            strcat(next->line,"rbss");
            strcat(next->line,", ");
            char rbssOffsetTemp[10];
            sprintf(rbssOffsetTemp,"%d", info->rbssOffset);
            strcat(next->line,rbssOffsetTemp);
        }

        next = getNextLine(topNode->code);

        strcat(next->line, "i2i ");
        strcat(next->line, registerName);
        strcat(next->line, " => ");
        strcat(next->line, attributionRegister);

        if(rightOperand->token_type != NATUREZA_IDENTIFICADOR && strcmp(rightOperand->token_value,AST_FUNCTIONCALL) != 0){
            return rightOperand->code;
        }
    }
    return topNode->code;
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
        return "FLOAT_ERROR";
    }
    return "";
}

char* newRegister(){
    char registerNumber[10];
    sprintf(registerNumber,"%d", registerIndex++);
    char* registerName = malloc(sizeof(char)*10);
    registerName[0] = '\0'; //strcat NEEDS this
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
    
    strcat(labelName, "L");
    strcat(labelName, labelNumber);

    return labelName;
}

struct code* newCode(){
    struct code* newCode = malloc(sizeof(struct code));
    newCode->previous = NULL;
    newCode->line = malloc(sizeof(char)*100);//size of string is 100
    newCode->line[0] = '\0';
    newCode->next = NULL;
    return newCode;
}

struct code* concatTwoCodes(struct code* executedFirst, struct code* executedSecond){
    struct code* codeIterator = NULL;
    struct code* returnCode = NULL;

    codeIterator = executedFirst;

    if(codeIterator == NULL){
        return executedSecond;
    }

    while(codeIterator->next != NULL){
        codeIterator = codeIterator->next;
    }
       
    codeIterator->next = executedSecond;

    if(executedSecond != NULL)
        executedSecond->previous = codeIterator;
    
    return executedFirst;

}

void updateNodeCodeIF(struct node* ifNode, struct node* condition, struct node* ifTrue){
    ifNode->code = newCode();
    char * rot = newLabel();
    char * rot2 = newLabel();
    patching(condition->code, rot, 1);
    patching(condition->code, rot2, 0);


    ifNode->code = concatTwoCodes(ifNode->code, condition->code);


    //nextline should have the last line of code
    struct code* nextLine = getNextLine(ifNode->code);
    

    strcat(nextLine->line, rot);
    strcat(nextLine->line, ":");

    ifNode->code = concatTwoCodes(ifNode->code, ifTrue->code);


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

    ifNode->code = concatTwoCodes(ifNode->code, condition->code);
    
    //nextline should have the last line of code
    struct code* nextLine = getNextLine(ifNode->code);


    strcat(nextLine->line, rot);
    strcat(nextLine->line, ":");
    
    ifNode->code = concatTwoCodes(ifNode->code, ifTrue->code);


    //nextline should have the last line of code
    nextLine = getNextLine(ifNode->code);

    
    strcat(nextLine->line, "jumpI => ");
    strcat(nextLine->line, end);


    nextLine->next = newCode();
    nextLine->next->previous = nextLine;
    nextLine = nextLine->next;  


    strcat(nextLine->line, rot2);
    strcat(nextLine->line, ":");

    ifNode->code = concatTwoCodes(ifNode->code, ifFalse->code);

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

    whileNode->code = concatTwoCodes(whileNode->code, condition->code);

    struct code* NextLine = getNextLine(whileNode->code);
    strcat(NextLine->line, Btrue);
    strcat(NextLine->line, ":");

    whileNode->code = concatTwoCodes(whileNode->code, commandsBlock->code);
    
    NextLine = getNextLine(whileNode->code);
    strcat(NextLine->line, "jumpI =>");
    strcat(NextLine->line, begin);

    NextLine = getNextLine(whileNode->code);
    strcat(NextLine->line, Bfalse);
    strcat(NextLine->line, ":");   

    free(begin);
    free(Btrue);
    free(Bfalse);


}

void updateCodeDOWHILE(struct node* whileNode, struct node* condition, struct node* commandsBlock){

    char* Btrue = newLabel();
    char* Bfalse = newLabel();
    patching(condition->code, Btrue, 1);
    patching(condition->code, Bfalse, 0);

    whileNode->code = newCode();
    strcat(whileNode->code->line, Btrue);
    strcat(whileNode->code->line, ":");

    whileNode->code = concatTwoCodes(whileNode->code, commandsBlock->code);
    whileNode->code = concatTwoCodes(whileNode->code, condition->code);

    struct code* NextLine = getNextLine(whileNode->code);
    strcat(NextLine->line, Bfalse);
    strcat(NextLine->line, ":");   

    free(Btrue);
    free(Bfalse);

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
    struct code* codeIterator = code;

    while(codeIterator){
        fixLine(codeIterator->line, par, replacement);
        codeIterator = codeIterator->next;
    }
}

struct code* removeCBR(struct code* code){
    if(code == NULL)
        return code;
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
            codeIterator->previous->next = codeIterator->next;
            free(codeIterator);
        }
        codeIterator = replaceCode;
    }

    return returnCode;
}

void liberaCode(struct code* code){
    if(code == NULL){
        return;
    }
    struct code* codeIterator = code;
    while(codeIterator->next){
        struct code* temp = codeIterator;
        codeIterator = codeIterator->next;
        if(temp->line)
            free(temp->line);
        free(temp);
    }
    if(code->line)
        free(code->line);
    free(code);
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
        //Dup is a temporary copy of the rest of the string so we can replace safely
        dup = strdup(token + 2);

        //Now we need to replace, then put the dup back on the string
        strcpy(token, replacement);
        //Thank god for \0 marking the end of strings
        strcat(token, dup);
    
        free(dup);
    }


}

struct code* makeReturnCode(struct node* expressionNode){
    struct code* code = newCode();
    if(expressionNode->registerTemp){
        strcat(code->line,"storeAI ");
        strcat(code->line, expressionNode->registerTemp);
        strcat(code->line," => rfp, 20 //store expression reg in return ");
    }
    else{ //if it is a literal and has no register
        char* expressionReg = newRegister();
        strcat(code->line,"loadAI ");
        strcat(code->line, expressionNode->child->child->token_value);
        strcat(code->line," => ");
        strcat(code->line, expressionReg);

        struct code* next = getNextLine(code);
        strcat(next->line,"storeAI ");
        strcat(next->line, expressionReg);
        strcat(next->line," => rfp, 20 //store expression reg in return ");
    }

    return code;
}

int getParameterCount(struct node* functionHead){
    struct node* functionCallArgumentList = functionHead->child->brother->brother->brother;
    int numberOfParameters = 0;

    if(numberOfChildren(functionCallArgumentList) != 0){
        struct node* functionArgumentElement = functionCallArgumentList;
        numberOfParameters++;
        while(numberOfChildren(functionArgumentElement) == 5){
            numberOfParameters++;
            functionArgumentElement = functionArgumentElement->child->brother->brother->brother->brother;
        }
    }

    return numberOfParameters;
}

//given a functionNode, adds it to the global functionlabels writes the label now. If the function is called "main", updates mainLabel
//Then adds the label of the function before the code of its commands
struct code* endFunctionCode(struct node* functionHead){
    char* name;
    if(numberOfChildren(functionHead) == 5){
        name = functionHead->child->brother->token_value; 
    } else if(numberOfChildren(functionHead) == 6){
        name = functionHead->child->brother->brother->token_value;
    }

    int isFunctionMain = 0;
    if(strcmp(name, "main") == 0){
        isFunctionMain = 1;
    }
    if(!isFunctionMain){
        struct code* code = newCode();
        char* regReturnAdress = newRegister();
        char* regRsp = newRegister();
        char* regRfp = newRegister();

        strcat(code->line, "loadAI ");
        strcat(code->line, "rfp, 0"); 
        strcat(code->line, " => ");
        strcat(code->line, regReturnAdress);
        strcat(code->line, " //start of end function code");
        
        struct code* next;
        next = getNextLine(code);
        strcat(next->line, "loadAI ");
        strcat(next->line, "rfp, 4"); 
        strcat(next->line, " => ");
        strcat(next->line, regRsp);

        next = getNextLine(code);
        strcat(next->line, "loadAI ");
        strcat(next->line, "rfp, 8"); 
        strcat(next->line, " => ");
        strcat(next->line, regRfp);

        next = getNextLine(code);
        strcat(next->line, "i2i ");
        strcat(next->line, regRsp); 
        strcat(next->line, " => rsp");

        next = getNextLine(code);
        strcat(next->line, "i2i ");
        strcat(next->line, regRfp); 
        strcat(next->line, " => rfp");

        next = getNextLine(code);
        strcat(next->line, "jump ");
        strcat(next->line, "=> ");
        strcat(next->line, regReturnAdress); 


        free(regReturnAdress);
        free(regRsp);
        free(regRfp);
        return code;
    }
    return NULL;
}

//given a functionNode, adds it to the global functionlabels writes the label now. If the function is called "main", updates mainLabel
//Then adds the label of the function before the code of its commands
void declareFunctionCode(struct node* functionHead){

    char* name;
    if(numberOfChildren(functionHead) == 5){
        name = functionHead->child->brother->token_value; 
    } else if(numberOfChildren(functionHead) == 6){
        name = functionHead->child->brother->brother->token_value;
    }

    char* label = newLabel();
    functionLabels = addFunctionLabelList(functionLabels, name, label);

    int isFunctionMain = 0;
    if(strcmp(name, "main") == 0){
        mainLabel = strdup(label);
        isFunctionMain = 1;
    }
    struct code* code = newCode();
    strcat(code->line, label);
    strcat(code->line, ":");

    if(!isFunctionMain){
        
        int numberOfParameters = getParameterCount(functionHead);
        int rspPointer = numberOfParameters*4 + 24; // on top of stack, for new variables
                                                    // 24 because 20 is the return

        struct code* next;
        next = getNextLine(code);
        strcat(next->line, "i2i rsp => rfp ");
        
        char numberOfArgumentsOffset[25];
        sprintf(numberOfArgumentsOffset, "%d", rspPointer);
        next = getNextLine(next);
        strcat(next->line, "loadAI rsp, ");
        strcat(next->line, numberOfArgumentsOffset);  
        strcat(next->line, " => rsp ");     

        
        //Now we need to stack each parameter and store them
        int stackOffset = 24;
        if(numberOfParameters > 0){
            //We are now inside the arguments, we need to get each register for each one and save it
            struct node* functionCallElement = functionHead->child->brother->brother->brother;
            while(numberOfChildren(functionCallElement) == 5){
                struct node* identifierNode = functionCallElement->child->brother->brother;
                struct symbolInfo* info = findSymbolInCurrentContext(identifierNode->token_value);

                char stackOffsetString[25];
                sprintf(stackOffsetString, "%d", stackOffset);

                next = getNextLine(code);
                strcat(next->line, "loadAI rfp, ");
                strcat(next->line, stackOffsetString);
                strcat(next->line, " => ");
                strcat(next->line, info->registerTemp); 

                next = getNextLine(code);

                sprintf(stackOffsetString, "%d", rspPointer);
                info->rfpOffset = rspPointer;

                strcat(next->line, "storeAI ");
                strcat(next->line, info->registerTemp);
                strcat(next->line, " => rfp, ");
                strcat(next->line, stackOffsetString); 
                strcat(next->line, " //store of "); 
                strcat(next->line, identifierNode->token_value); 

                stackOffset += 4;
                rspPointer += 4;
                functionCallElement = functionCallElement->child->brother->brother->brother->brother;
            }
            struct node* identifierNode = functionCallElement->child->brother->brother;
            
            struct symbolInfo* info = findSymbolInCurrentContext(identifierNode->token_value);
            char stackOffsetString[25];
            sprintf(stackOffsetString, "%d", stackOffset);

            next = getNextLine(code);
            strcat(next->line, "loadAI rfp, ");
            strcat(next->line, stackOffsetString);
            strcat(next->line, " => ");
            strcat(next->line, info->registerTemp); 

            next = getNextLine(code);
            sprintf(stackOffsetString, "%d", rspPointer);
            info->rfpOffset = rspPointer;

            strcat(next->line, "storeAI ");
            strcat(next->line, info->registerTemp);
            strcat(next->line, " => rfp, ");
            strcat(next->line, stackOffsetString); 
            strcat(next->line, " //store of "); 
            strcat(next->line, identifierNode->token_value); 
            rspPointer += 4;

        }

        rfpOffset = rspPointer; //for new variables
    }


    //ACTUAL CODE
    functionHead->code = concatTwoCodes(code, functionHead->code);

}

//Given a functionCall node, writes the code for it
struct code* writeFunctionCall(struct node* functionCall){
    //Prepare to update RSP by putting rfpoffset in a string
    char RSPUpdate[33];
    sprintf(RSPUpdate, "%d", rfpOffset);

    dynamicLink = rfpOffset;

    //Creates a new line updating RSP
    struct code* code = newCode();
    strcat(code->line, "addI rsp, ");
    strcat(code->line, RSPUpdate);
    strcat(code->line, " => rsp");


    //Gets the value to jump the PC to after the function
    int jumpPCOffset = getFunctionArgumentsNumber(functionCall) + 9; 
                                                                    // 1 - return adress
                                                                    // 2 - rsp save
                                                                    // 3 - rfp save
                                                                    // 4 - static link load
                                                                    // 5 - static link
                                                                    // 6 - dynamic link load
                                                                    // 7 - dynamic link
                                                                    // 8 - jumpI
                                                                    // 9 - save return value to reg
    char jumpPCString[33];
    sprintf(jumpPCString, "%d", jumpPCOffset);
    char* jumpPCLabel = newRegister();

    //Creates the line to add PC an saves it in a new register
    struct code* next = getNextLine(code);
    strcat(next->line, "addI rpc, ");
    strcat(next->line, jumpPCString);
    strcat(next->line, " => ");
    strcat(next->line, jumpPCLabel);

    //RSP and rfpOffset now have the future rfp of the function to be called
    next = getNextLine(code);
    strcat(next->line, "storeAI ");
    strcat(next->line, jumpPCLabel);
    strcat(next->line, " => rsp, 0");
    strcat(next->line, " //return adress save");

    //Now we save the RSP and RFP in the base of the frame of the future function
    next = getNextLine(code);
    strcat(next->line, "storeAI rsp => rsp, 4");
    strcat(next->line, " //rsp save");
    next = getNextLine(code);
    strcat(next->line, "storeAI rfp => rsp, 8");
    strcat(next->line, " //rfp save");

    //We need to save the dynamic link (varies, as it is the base of the activation record of the calling function)
    //and static link (which will always be zero)
    char* dynamicLinkReg = newRegister();
    char* staticLinkReg = newRegister();
    //Static Link
    next = getNextLine(code);
    strcat(next->line, "loadI 0 => ");
    strcat(next->line, staticLinkReg);
    
    next = getNextLine(code);
    strcat(next->line, "storeAI ");
    strcat(next->line, staticLinkReg);
    strcat(next->line, " => rsp, 12");
    strcat(next->line, " //static link");

    //Dynamic Link
    next = getNextLine(code);
    strcat(next->line, "loadI ");

    char dynamicLinkString[33];
    sprintf(dynamicLinkString, "%d", dynamicLink);
    strcat(next->line, dynamicLinkString);
    strcat(next->line, " => ");
    strcat(next->line, dynamicLinkReg);
    
    next = getNextLine(code);
    strcat(next->line, "storeAI ");
    strcat(next->line, dynamicLinkReg);
    strcat(next->line, " => rsp, 16");
    strcat(next->line, " //dynamic link");

    free(dynamicLinkReg);
    free(staticLinkReg);

    //Now we need to stack each parameter and store them
    int stackOffset = 24;
    struct node* functionCallArguments = functionCall->child->brother->brother;
    if(numberOfChildren(functionCallArguments) == 1){
        struct node* functionCallArgumentsList = functionCallArguments->child;
        //We are now inside the arguments, we need to get each register for each one and save it
        while(numberOfChildren(functionCallArgumentsList) == 3){
            
            struct node* expression = functionCallArgumentsList->child->child;
            char* reg; //functionDeclarationArgumentList
            
            //If expression is a literal we need to put it in a register first
            if(strcmp(expression->token_value, AST_LITERAL) == 0){
                reg = newRegister();
                next = getNextLine(code);
                strcat(next->line, "loadAI ");
                strcat(next->line, calculateCodeGenValue(expression->child));
                strcat(next->line, " => ");
                strcat(next->line, reg);

            }
            else if(expression->token_type == NATUREZA_IDENTIFICADOR){
                struct symbolInfo* info =  findSymbolInContexts(expression->token_value);
                if(info->nature == NATUREZA_FUNC){
                    reg = expression->registerTemp;
                } else reg = info->registerTemp;
            }
            else {
                reg = expression->registerTemp;
            }

            //Gets the offset to be saved
            char stackOffsetString[33];
            sprintf(stackOffsetString, "%d", stackOffset);
            
            next = getNextLine(code);
            strcat(next->line, "storeAI ");
            strcat(next->line, reg);
            strcat(next->line, " => rsp, ");
            strcat(next->line, stackOffsetString);
            strcat(next->line, " //store of parameter");
            stackOffset +=4; //updates offset

            functionCallArgumentsList = functionCallArgumentsList->child->brother->brother;

        }

        //We are now on the last node

        //Gets register where the expression is saved        
        struct node* expression = functionCallArgumentsList->child->child;
        char* reg;
        
        //If expression is a literal we need to put it in a register first
        if(strcmp(expression->token_value, AST_LITERAL) == 0){
            reg = newRegister();
            next = getNextLine(code);
            strcat(next->line, "loadAI ");
            strcat(next->line, calculateCodeGenValue(expression->child));
            strcat(next->line, " => ");
            strcat(next->line, reg);

        }
        else if(expression->token_type == NATUREZA_IDENTIFICADOR){
            struct symbolInfo* info =  findSymbolInContexts(expression->token_value);
            if(info->nature == NATUREZA_FUNC){
                reg = expression->registerTemp;
            } else reg = info->registerTemp;
        }
        else reg = expression->registerTemp;

        
        //Gets the offset to be saved
        char stackOffsetString[33];
        sprintf(stackOffsetString, "%d", stackOffset);

        next = getNextLine(code);
        strcat(next->line, "storeAI ");
        strcat(next->line, reg);
        strcat(next->line, " => rsp, ");
        strcat(next->line, stackOffsetString);
        strcat(next->line, " //store of parameter");
        stackOffset +=4; //updates offset

        
    }


    //Everything is now stacked,, now we JUMP
    char* functionLabel = findLabel(functionLabels, functionCall->child->token_value);
    next = getNextLine(code);
    strcat(next->line, "jumpI => ");
    strcat(next->line, functionLabel);

    //Now all that is needed to do is load the value returned by the function
    //The value will be in the stackOffset
    char* functionValue = newRegister();

    next = getNextLine(code);
    strcat(next->line, "loadAI rsp, 20");
    strcat(next->line, " => ");
    strcat(next->line, functionValue);
    strcat(next->line, " //save return from function on reg");

    functionCall->registerTemp = functionValue;
    functionCall->code = code;

}

//Makes the code jump to the "main function" at the start of the code
struct code* addJumpToFirstLine(struct code* program){
    if(mainLabel != NULL){

    struct code* firstLine = newCode();
    strcat(firstLine->line, "jumpI => ");
    strcat(firstLine->line, mainLabel);
    return concatTwoCodes(firstLine, program); 
    
    }

    else return program;
}


//Given a functionCall node, writes the code for it
struct code* receiveFunctionCall(struct node* functionCall){

}

