all: etapa4
 
etapa4 : lex.yy.c main.c bison ASTree.c symbolTable.c auxiliar.c symbolTableInterface.c
	gcc -o etapa4 main.c lex.yy.c parser.tab.c ASTree.c symbolTable.c auxiliar.c symbolTableInterface.c -lfl

lex.yy.c : scanner.l bison
	flex scanner.l
	
bison: parser.y
	bison -v -d parser.y