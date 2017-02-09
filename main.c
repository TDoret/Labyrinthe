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
    int ligMax = 25;
    int colMax = 25;
    key_t key;              // cle d'accès à la structure IPC

    // generation de la cle
    key = 5678;

    printf("%s\n", "Début Programme");
    //je genere les espaces de mem partage
    shm = generateMatAdj_SHM(key);
    sem = generateSem(key);

    //Je genere le laby
    generateMaze(ligMax, colMax, sem);

    //je bloque l'accès a la shm pour les bots
	printf("Press Any Key to start maze\n");
    P(sem);
	getchar();
    //Je libère, le jeu peut commencer
    V(sem);

    printf("Maze in progress ...\n");
    printf("Press Any Key to close maze\n");
    getchar();
    //je detruis la shm
    destroyMatAdj_SHM(shm);

    printf("\n%s\n", "Fin Programme");

    return EXIT_SUCCESS;
}
