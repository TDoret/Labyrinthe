#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include "function.h"

int main() {
    int shm;            // identifiant de la memoire partagee
    int sem;               // identifiant du semaphore
    int ligMax = 10;
    int colMax = 10;
    key_t key;              // cle d'accès à la structure IPC

    // generation de la cle
    key = 5678;

    printf("%s\n", "Début Programme");
    shm = generateMatAdj_SHM(key);
    sem = generateSem(key);

    generateMaze(ligMax, colMax, sem);

	printf("Press Any Key to close maze\n");
	getchar();
    destroyMatAdj_SHM(shm);
    printf("\n%s\n", "Fin Programme");

    return EXIT_SUCCESS;
}
