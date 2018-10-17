all: etapa3
 
etapa3 : lex.yy.c main.c bison ASTree.c symbolTree.c auxiliar.c
	gcc -o etapa3 main.c lex.yy.c parser.tab.c ASTree.c symbolTable.c auxiliar.c -lfl

lex.yy.c : scanner.l bison
	flex scanner.l
	
bison: parser.y
	bison -v -d parser.y