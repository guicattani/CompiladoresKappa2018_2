all: etapa6
 
etapa6 : lex.yy.c main.c bison ASTree.c symbolTable.c auxiliar.c symbolTableInterface.c codeGen.c
	gcc -o etapa6 main.c lex.yy.c parser.tab.c ASTree.c symbolTable.c auxiliar.c symbolTableInterface.c codeGen.c -lfl

lex.yy.c : scanner.l bison
	flex scanner.l
	
bison: parser.y
	bison -v -d parser.y