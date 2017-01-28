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
  	int rowMax = 3;
  	int ColMax = 3;

	printf("%s\n","Hello World !");
	shm = generateMatAdj_SHM(argc, argv[0], argv[1]);

	generateMaze(rowMax,ColMax, shm);

	//exemple de lecture dans la SHM
	printf("%s\n", "----Read In SHM----");
	printf("%d\n",getMatAdj_SHM(shm, 0, 0, rowMax));
	//sleep(5);
	destroyMatAdj_SHM(shm);

	printf("\n%s\n","Fin Programme");
	
    return EXIT_SUCCESS;
}

