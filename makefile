all: etapa2


etapa2 : lex.yy.c main.c bison
	gcc -o etapa1 main.c lex.yy.c parser.tab.c -lfl

lex.yy.c : scanner.l
	flex scanner.l

 
bison: parser.y
	bison -v -d parser.y