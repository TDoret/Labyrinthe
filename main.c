#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include "function.h"

int main(int argc, char *argv[])
{
  	int shm;			// identifiant de la memoire partagee
  	int ligMax = 3;
  	int colMax = 3;

	printf("%s\n","Début Programme");
	shm = generateMatAdj_SHM(argc, argv[0], argv[1]);

	generateMaze(ligMax,colMax, shm);

	//exemple de lecture dans la SHM
	printf("%s\n", "----Lecture dans SHM----");
	printf("%d\n",getMatAdj_SHM(shm, 0, 0, ligMax));
	//sleep(5);
	destroyMatAdj_SHM(shm);

	printf("\n%s\n","Fin Programme");

	return EXIT_SUCCESS;
}
