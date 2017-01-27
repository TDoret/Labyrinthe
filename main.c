#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h>
#include <sys/types.h>
#include "function.h"

int main(int argc, char *argv[])
{
  	int shm;                  			// identifiant de la memoire partagee
  	int *adjM = NULL;      				// pointeur d'attachement shared memory == matrice d'adjacence
  	int value = 1;

	printf("%s\n","Hello World !");
	shm = generateSHM(argc, argv[0], argv[1]);
	printf("%s\n", "----Write In SHM---");
	writeSHM(shm, adjM, value, 0, 0, 4);
	printf("%s\n", "----Read In SHM----");
	readSHM(shm, adjM, 0, 0, 4);
	destroySHM(shm);
	generateMaze(3,4);
	printf("\n%s\n","Fin Programme");
	
    return EXIT_SUCCESS;
}

