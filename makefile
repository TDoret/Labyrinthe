#build an executable named myprog from main.c
#make -f makeFile to build
#make -f makeFile clean to remove *.o
#make -f makeFile mrproper to remove all project + exe

all: myprog

myprog: main.o function.o
	gcc -o myprog main.o function.o


function.o: function.h function.c 
	gcc -o function.o -c function.c -W -Wall -ansi -pedantic -std=c99

main.o: main.c function.h
	gcc -o main.o -c main.c -W -Wall -ansi -pedantic -std=c99

clean:
	rm -rf *.o

mrproper: clean
	rm -rf myprog
