
CC=gcc

compileAll: driver.o lexer.o set.o parser.o stack.o linklist.o ast.o symbolTable.o
	gcc -o stage1exe driver.o lexer.o set.o parser.o stack.o linklist.o ast.o symbolTable.o

driver.o: driver.c _LEXER.h _LEXERDEF.h _SET.h _PARSER.h _PARSERDEF.h _STACK.h _LINKEDLIST.h _SEMANTIC.h _SEMANTICDEF.h
	gcc -g -c driver.c
	
set.o: set.c _SET.h
	gcc -g -c set.c

lexer.o: lexer.c _LEXER.h _LEXERDEF.h
	gcc -g -c lexer.c

parser.o: parser.c _PARSER.h _PARSERDEF.h
	gcc -g -c parser.c

stack.o: set.c _LEXER.h _LEXERDEF.h _SET.h _PARSER.h _PARSERDEF.h _STACK.h
	gcc -g -c stack.c

linklist.o: linklist.c _LINKEDLIST.h
	gcc -g -c linklist.c

ast.o: ast.c _SEMANTIC.h _SEMANTICDEF.h
	gcc -g -c ast.c

symbolTable.o: symbolTable.c  _SEMANTICDEF.h _LEXER.h _LEXERDEF.h
	gcc -g -c symbolTable.c

clean:
	rm *.o stage1exe

run: stage1exe
	./stage1exe

gdb: stage1exe
	xterm -e gdb ./stage1exe
