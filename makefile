assymbler: main.o errors.o
	gcc -ansi -pedantic -Wall -g main.o errors.o -o assymbler
main.o: main.c errors.h
	gcc -c -ansi -pedantic -Wall main.c -o main.o
errors.o: errors.c errors.h
	gcc -c -ansi -pedantic -Wall errors.c -o errors.o