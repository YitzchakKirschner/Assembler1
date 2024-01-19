assembler: main.o errors.o macro_extracter.o
	gcc -ansi -pedantic -Wall -g main.o errors.o macro_extracter.o -o assembler
main.o: main.c errors.h macro_extracter.h
	gcc -c -ansi -pedantic -Wall main.c -o main.o
errors.o: errors.c errors.h
	gcc -c -ansi -pedantic -Wall errors.c -o errors.o
macro_extracter.o: macro_extracter.c macro_extracter.h errors.h
	gcc -c -ansi -pedantic -Wall macro_extracter.c -o macro_extracter.o