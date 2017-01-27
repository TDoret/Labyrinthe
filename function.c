#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/shm.h>

#include <unistd.h>
#include <sys/types.h>
#include "function.h"
#include "pile.h"


#define LG_CHAINE 256



/*
1-	Haut
2-	Droite
3-	Bas
4-	Gauche
i = ligne
j = colonne*/

void SetCursorPos(int YPos, int XPos)
{
 	printf("[%d;%d]", YPos+1, XPos+1);
 	//printf("%c[%d;%d]", 0x1B, YPos, XPos);
}


void generateMaze(int M, int N)
{
	int tabV[M][N];
	int tabX[M+1][N];
	int tabY[M][N+1]; 
	//int matAdj[M][N];
	int a,b;

	int isValidDirect = 0;
	int isValidUp = 0;
	int isValidRight = 0;
	int isValidDown = 0;
	int isValidLeft = 0;	
	int dir = 0;


	for(a = 0; a < M; a++) {
		for(b = 0; b < N; b++) {
			tabV[a][b] = 0;
		}
	}

	for(a = 0; a < M+1; a++) {
		for(int b = 0; b < N; b++) {
			tabX[a][b] = 0;
		}
	}

	for(a = 0; a < M; a++) {
		for(b = 0; b < N+1; b++) {
			tabY[a][b] = 0;
		}
	}

	srand(time(NULL));
	int i = (rand() % M);
	int j = (rand() % N);
	int cpt = 0;
	int nbCases = M*N;
    Pile *maPile = initialiser();
	Coord coordTemp;
	printf("%d:%d\n",i,j);
	tabV[i][j] = 1;
	printf("nb Cases: %d\n",nbCases);
	while (cpt  < nbCases) {
		//todo tant que inférieur à nb cases (M*N)
		//on checke qu'il puisse aller dans une direction random
		isValidDirect = 0;
		isValidUp = 0;
		isValidRight = 0;
		isValidDown = 0;
		isValidLeft = 0;	
		dir = 0;
		//tant que la direction n'est pas bonne
		do {
			//si toutes les directions ne sont pas bonnes, on retourne au dernier noeud
			printf("je commence le do\n");
			printf("position de V=>%d:%d\n",i,j);
			printf("^:%d/>:%d/v:%d/<:%d \n",isValidUp,isValidRight,isValidDown,isValidLeft);
			if((isValidUp == -1) && (isValidRight == -1) && (isValidDown == -1) && (isValidLeft == -1) )
			{
				printf("%s\n","Aucune direction Possible !");
				coordTemp = depiler(maPile);
				i = coordTemp.x;
				j = coordTemp.y;
				isValidUp = 0;
				isValidRight = 0;
				isValidDown = 0;
				isValidLeft = 0;
				printf("Je remonte le noeud=> %d:%d\n", i,j);
				break;
			}
			printf("%s\n","Choix direction");
			dir = (rand() % 4)+1;
			printf("dir = %d\n", dir);
			//checker si c'est pas borné
			//faire une liste qui stocke les coordonnées courantes avant de se déplacer
			//on check si on a pas déjà visité la case vers où on veut aller
			printf("si pas borné\n");
			switch(dir) {
				case 1:
					if(i > 0) {
						isValidDirect = 1; 
						printf("petit check oklm\n");
						if(tabV[i-1][j] != 0)
						{
							isValidDirect = 0;
							isValidUp = -1;
						}
					}
					else
						isValidUp = -1;
					break;
				case 2: 
					if(j < M-1) {
						isValidDirect = 1;
						printf("petit check oklm\n");
						if(tabV[i][j+1] != 0)
						{
							isValidDirect = 0;
							isValidRight = -1;
						}
					}
					else
						isValidRight = -1;
					break;
				case 3: 
					if (i < N-1) {
						isValidDirect = 1;
						printf("petit check oklm\n");
						if(tabV[i+1][j] != 0)
						{
							isValidDirect = 0;
							isValidDown = -1;
						}
					}
					else
						isValidDown = -1;
					break;
				case 4: 
					if (j > 0) {
						isValidDirect = 1;
						printf("petit check oklm\n");
						if(tabV[i][j-1] != 0)
						{
							isValidDirect = 0;
							isValidLeft = -1;
						}
					}
					else
						isValidLeft = -1;
					break;
				default: 
					break;
			}
		}while(isValidDirect == 0);
		tabV[i][j] = 1;
		//si on est sorti, peut casser le mur
		if (isValidDirect == 1) {
			switch (dir) {
				case 1:
					//on casse le mur
					tabX[i][j] = 1;
					//on stocke la position courante
					coordTemp.x = i;
					coordTemp.y = j;
					empiler(maPile, coordTemp);
					//on se déplace
					i--;
					//on incrémente le compteur
					cpt ++;
					printf("je monte !\n");
					break;
				case 2:
					tabY[i][j+1] = 1;
					coordTemp.x = i;
					coordTemp.y = j;
					empiler(maPile, coordTemp);
					j++;
					//on incrémente le compteur
					cpt ++;
					printf("je vais à droite !\n");
					break;
				case 3:
					tabX[i+1][j] = 1;
					coordTemp.x = i;
					coordTemp.y = j;
					empiler(maPile, coordTemp);
					i++;
					//on incrémente le compteur
					cpt ++;
					printf("je vais en bas !\n");
					break;
				case 4:
					tabY[i][j] = 1;
					coordTemp.x = i;
					coordTemp.y = j;
					empiler(maPile, coordTemp);
					j--;
					//on incrémente le compteur
					cpt ++;
					printf("je vais à gauche !\n");
					break;
				default:
					break;
			}
			printf("------Fin de l'etape : %d--------\n", cpt);
		}
	}
	printf("%s\n", "Display Maze ?");
	//system("clear");
	printf("%s\n", "--TabV--");
	for(a=0;a<M;a++)
	{
		for(b=0;b<N;b++)
		{
			printf("%d", tabV[a][b]);	
			//printf("%c", '|');
		}
		printf("\n");
	}
	printf("%s\n", "--TabX--");
	for(a=0;a<M+1;a++)
	{
		for(b=0;b<N;b++)
		{
			printf("%d", tabX[a][b]);	
			//printf("%c", '|');
		}
		printf("\n");
	}
	printf("%s\n", "--TabY--");
	//system("clear");
	for(a=0;a<M;a++)
	{
		for(b=0;b<N+1;b++)
		{
			printf("%d", tabY[a][b]);	
			//printf("%c", '|');
		}
		printf("\n");
	}
}

int generateSHM(int argc, char *argv0,  char *argv1)
{
	key_t key;                // cle d'accès à la structure IPC
  	int shm; 				// identifiant de la memoire partagee

  	if (argc != 2) {
    	fprintf(stderr, "Syntaxe : %s fichier_clé \n", argv0);
    	exit(EXIT_FAILURE);
  	}

    // generation de la cle
    if ((key = ftok(argv1, 0)) == -1) {
      perror("ftok");
      exit(EXIT_FAILURE);
    }

    // creation du segment de memoire partagee
    if ((shm = shmget(key, LG_CHAINE, IPC_CREAT | 0600)) == -1) {
      perror("shmget");
      exit(EXIT_FAILURE);
    }
    printf("Generate SHM id: %d\n", shm);
    return shm;

}

void writeSHM(int shm, int * chaine, int value, int row, int column, int rowMax)
{
	// attachement du segment shm sur le pointeur *chaine
    if ((chaine = shmat(shm, NULL, 0)) == (void *)-1) {
      perror("shmat");
      exit(EXIT_FAILURE);
    }

    chaine[row * rowMax + column] = value;
    //fprintf(stdout, "> ");
    //fget(value, sizeof(int), stdin);
}

void readSHM(int shm, int * chaine, int row, int column, int rowMax)
{
    // attachement de la memoire partagee au pointeur *chaine
    if ((chaine = shmat(shm, NULL, SHM_RDONLY)) == (void *)-1) {
    	perror("shmat");
    	exit(EXIT_FAILURE);
    }

    printf("%d\n", chaine[row * rowMax + column]);

}

void destroySHM(int shm)
{
	printf("Destroy SHM id: %d\n", shm);
	shmctl(shm, IPC_RMID, NULL);
}









