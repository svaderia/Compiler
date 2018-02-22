run: exe
	./exe

gdb: exe
	xterm -e gdb ./exe

compileAll: set.o
	gcc -o exe set.o

set.o: set.c _SET.h
	gcc -g -c set.c

clean:
	rm *.o exe