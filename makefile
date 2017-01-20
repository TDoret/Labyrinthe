#build an executable named myprog from main.c
#make -f makeFile to build
#make -f makeFile clean to remove *.o
#make -f makeFile mrproper to remove all project + exe

all: myprog

myprog: main.o function.o file.o
	gcc -o myprog main.o function.o file.o


function.o: function.h function.c file.h
	gcc -o function.o -c function.c -W -Wall -ansi -pedantic -std=c99

file.o: file.h file.c 
	gcc -o file.o -c file.c -W -Wall -ansi -pedantic -std=c99

main.o: main.c function.h file.h
	gcc -o main.o -c main.c -W -Wall -ansi -pedantic -std=c99

clean:
	rm -rf *.o

mrproper: clean
	rm -rf myprog
