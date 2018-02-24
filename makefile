
CC=gcc

run: compileAll
	./exe.out

gdb: compileAll
	xterm -e gdb ./exe.out

compileAll: driver.o lexer.o set.o
	gcc -o exe.out driver.o lexer.o set.o

driver.o: driver.c _LEXER.h _LEXERDEF.h _SET.h
	gcc -g -c driver.c
	
set.o: set.c _SET.h
	gcc -g -c set.c

lexer.o: lexer.c _LEXER.h _LEXERDEF.h
	gcc -g -c lexer.c

clean:
	rm *.o exe