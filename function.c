#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/shm.h>

#include <unistd.h>
#include <sys/types.h>
#include "function.h"
#include "file.h"


#define LG_CHAINE 256



/*1-	Haut 
2-	Droite
3-	Bas
4-	Gauche

i = ligne
j = colonne*/

void SetCursorPos(int YPos, int XPos)
{
 	printf("\033[%d;%dH", YPos+1, XPos+1);
 	//printf("%c[%d;%df",0x1B,YPos,XPos);
}



void generateMaze(int M, int N)
{
	int tabV[M][N];
	int tabX[M][N+1];
	int tabY[M+1][N]; 

	srand(time(NULL));
	int i = rand() % M-1;
	int j = rand() % N-1;
	int cpt = 0;
	int nbCases = M*N;
	File *maFile = initialiser();

	Coord coordTemp;

		tabV[i][j] = 1;

		while (cpt  <= nbCases) {
			//todo tant que inférieur à nb cases (M*N)
			//on checke qu'il puisse aller dans une direction random
			int isValidDirect = 0;
			int isValidUp = 0;
			int isValidRight = 0;
			int isValidDown = 0;
			int isValidLeft = 0;	
			int dir = 0;
			//tant que la direction N'est pas bonne
			do {
				//si toutes les directions ne sont pas bonnes, on retourne au dernier noeud
				if(isValidUp == -1 && isValidRight == -1 && isValidDown == -1 && isValidLeft == -1)
				{
					coordTemp = defiler(maFile);
					i = coordTemp.x;
					j = coordTemp.y;
					break;
				}
				dir = (rand() % 4) + 1;
				//checker si c'est pas borné
				switch(dir) {
					case 1:
						if(i > 0)
							isValidDirect = 1; 
						else
							isValidUp = -1;
						break;
					case 2: 
						if(j < N-1)
							isValidDirect = 1;
						else
							isValidRight = -1;
						break;
					case 3: 
						if (i < N-1)
							isValidDirect = 1;
						else
							isValidDown = -1;
						break;
					case 4: 
						if (j > 0)
							isValidDirect = 1;
						else
							isValidLeft = -1;
						break;
					default: 
						break;
				}

				if (isValidDirect == 1) {
					//faire une liste qui stocke les coordonnées courantes avant de se déplacer
					//on check si on a pas déjà visité la case vers où on veut aller
					switch(dir) {
						case 1:
							if(tabV[i-1][j] != 0)
							{
								isValidDirect = 0;
								isValidUp = -1;
							}
							break;
						case 2:
							if(tabV[i][j+1] != 0)
							{
								isValidDirect = 0;
								isValidRight = -1;
							}
							break;
						case 3:
							if(tabV[i+1][j] != 0)
							{
								isValidDirect = 0;
								isValidDown = -1;
							}
							break;
						case 4:
							if(tabV[i][j-1] != 0)
							{
								isValidDirect = 0;
								isValidLeft = -1;
							}
							break;
						default:
							break;
					}
				}

			}while(isValidDirect == 0);

			//si on est sorti, peut casser le mur
			if (isValidDirect == 1) {
				switch (dir) {
					case 1:
						//on casse le mur
						tabX[i][j] = 1;
						//on stocke la position courante
						coordTemp.x = i;
						coordTemp.y = j;
						enfiler(maFile, coordTemp);
						//on se déplace
						i++;
						//on incrémente le compteur
						cpt ++;
						break;
					case 2:
						tabY[i][j+1] = 1;
						coordTemp.x = i;
						coordTemp.y = j;
						enfiler(maFile, coordTemp);
						j++;
						//on incrémente le compteur
						cpt ++;
						break;
					case 3:
						tabX[i+1][j] = 1;
						coordTemp.x = i;
						coordTemp.y = j;
						enfiler(maFile, coordTemp);
						i--;
						//on incrémente le compteur
						cpt ++;
						break;
					case 4:
						tabY[i][j] = 1;
						coordTemp.x = i;
						coordTemp.y = j;
						enfiler(maFile, coordTemp);
						j--;
						//on incrémente le compteur
						cpt ++;
						break;
					default:
						break;
				}
			}
		}

		int a,b;
		printf("%s\n", "Display Maze ?");
		system("clear");

		for(a=0;a<M-1;a++)
		{
			for(b=0;b<N;b++)
			{
				SetCursorPos(a,b);
				printf("%c", '|');

			}
		}
		for(a=0;a<M;a++)
		{
			for(b=0;b<N-1;b++)
			{
				SetCursorPos(a,b);
				printf("%c", '_');

			}
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









