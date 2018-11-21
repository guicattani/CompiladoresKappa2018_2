#include "codeGen.h"

int registerIndex = 0;
int labelIndex = 0;

//rfpOffset have been moved to symbolTable because it changes with contex
extern int rfpOffset;
int rbssOffset = 0;

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
            if(rightOperand->token_type != NATUREZA_IDENTIFICADOR)
                attributionRegister = rightOperand->registerTemp;
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
            if(leftOperand->token_type != NATUREZA_IDENTIFICADOR)
                attributionRegister = leftOperand->registerTemp;
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
            if(leftOperand->token_type != NATUREZA_IDENTIFICADOR)
                leftAttributionRegister = leftOperand->registerTemp;
            else
                leftAttributionRegister = leftRegisterFromIdentifier;

            if(rightOperand->token_type != NATUREZA_IDENTIFICADOR)
                rightAttributionRegister = rightOperand->registerTemp;
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
            registerName = rightInfo->registerTemp;
        }
        else{
            registerName = rightOperand->registerTemp;
            concatTwoCodes(rightOperand->code, topNode->code);
        }

        strcat(topNode->code->line,"storeAI ");
        strcat(topNode->code->line,registerName);
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

        struct code* next = getNextLine(topNode->code);

        strcat(next->line, "i2i ");
        strcat(next->line, registerName);
        strcat(next->line, " => ");
        strcat(next->line, attributionRegister);

        if(rightOperand->token_type != NATUREZA_IDENTIFICADOR){
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
