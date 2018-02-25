
CC=gcc

run: exe.out
	./exe.out

gdb: exe.out
	xterm -e gdb ./exe.out

compileAll: driver.o lexer.o set.o parser.o stack.o linklist.o
	gcc -o exe.out driver.o lexer.o set.o parser.o stack.o linklist.o

driver.o: driver.c _LEXER.h _LEXERDEF.h _SET.h _PARSER.h _PARSERDEF.h _STACK.h _LINKEDLIST.h
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

clean:
	rm *.o exe.out