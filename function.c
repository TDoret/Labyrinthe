#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include "function.h"
#include "pile.h"


#define LG_CHAINE 1024

int *adjM = NULL;    // pointeur d'attachement shared memory == matrice d'adjacence

/*
1-	Haut
2-	Droite
3-	Bas
4-	Gauche
i = ligne
j = colonne*/

void SetCursorPos(int YPos, int XPos) {
    printf("[%d;%d]", YPos + 1, XPos + 1);
    //printf("%c[%d;%d]", 0x1B, YPos, XPos);
}


void generateMaze(int M, int N, int sem) {
    //tab pour générer le lab
    int tabV[M][N];
    int tabX[M + 1][N];
    int tabY[M][N + 1];
    int matAdj[M][N];
    int a, b;
    int dirTab[4];

    //flag pour le check des directions;
    int isValidDirect = 0;
    int isValidUp = 0;
    int isValidRight = 0;
    int isValidDown = 0;
    int isValidLeft = 0;
    int lastdir = 0;
    int dir = 0;

    //struct utile pour stocker le parcours de l'algo
    srand(time(NULL));
    int i = (rand() % M);
    int j = (rand() % N);
    int cpt = 0;
    int nbCases = M * N;
    Pile *maPile = initialiser();
    Coord coordTemp;

    //j'initialise les tabs à 0
    for (a = 0; a < M; a++) {
        for (b = 0; b < N; b++) {
            tabV[a][b] = 0;
        }
    }

    for (a = 0; a < M; a++) {
        for (b = 0; b < N; b++) {
            matAdj[a][b] = 0;
        }
    }

    for (a = 0; a < M + 1; a++) {
        for (int b = 0; b < N; b++) {
            tabX[a][b] = 0;
        }
    }

    for (a = 0; a < M; a++) {
        for (b = 0; b < N + 1; b++) {
            tabY[a][b] = 0;
        }
    }

    /* ----- DEBUT DE L'ALGO -------*/

    printf("%d:%d\n", i, j);
    tabV[i][j] = 1;

    //nbCase a parcourir pour arriver au bout de l'algo
    printf("nb Cases: %d\n", nbCases);
    while (cpt < nbCases - 1) {
        //todo tant que inférieur à nb cases (M*N)
        //on checke qu'il puisse aller dans une direction random
        isValidDirect = 0;
        isValidUp = 0;
        isValidRight = 0;
        isValidDown = 0;
        isValidLeft = 0;
        dir = 0;
		dirTab[0] = 1;
		dirTab[1] = 2;
		dirTab[2] = 3;
		dirTab[3] = 4;


        //tant que la direction n'est pas bonne
        do {
            printf("je commence le do\n");
            printf("position de V=>%d:%d\n", i, j);
            tabV[i][j] = 1;
            printf("^:%d/>:%d/v:%d/<:%d \n", isValidUp, isValidRight, isValidDown, isValidLeft);

            //si toutes les directions ne sont pas bonnes, on retourne au dernier noeud
            if ((isValidUp == -1) && (isValidRight == -1) && (isValidDown == -1) && (isValidLeft == -1)) {
                printf("%s\n", "Aucune direction Possible !");
                coordTemp = depiler(maPile);
                i = coordTemp.x;
                j = coordTemp.y;
                lastdir = coordTemp.lastDir;
                isValidUp = 0;
                isValidRight = 0;
                isValidDown = 0;
                isValidLeft = 0;
                printf("Je remonte le noeud=> %d:%d\n", i, j);
                break;
            }

            //on ajoute la direction inversé du predecesseur dans le doute qu'on ne se déplace pas ;)
            matAdj[i][j] = lastdir;
            //printf("%s\n", "----Ecrire dans SHM---");
            setMatAdj_SHM(sem, matAdj[i][j], i, j, M);

            //on choisi la direction
            rand: dir = dirTab[rand() % 4];
            if(dir == -1)
            	goto rand;

            printf("%s\n", "Choix direction");
            printf("dir = %d\n", dir);

            //checker si c'est pas borné
            //faire une liste qui stocke les coordonnées courantes avant de se déplacer
            //on check si on a pas déjà visité la case vers où on veut aller
            printf("si pas borné\n");
            switch (dir) {
                case 1:
                    if (i > 0) {
                        isValidDirect = 1;
                        printf("petit check oklm\n");
                        if (tabV[i - 1][j] != 0) {
                            isValidDirect = 0;
                            isValidUp = -1;
                            dirTab[0] = -1;
                        }
                    } else
                        isValidUp = -1;
                    break;
                case 2:
                    if (j < M - 1) {
                        isValidDirect = 1;
                        printf("petit check oklm\n");
                        if (tabV[i][j + 1] != 0) {
                            isValidDirect = 0;
                            isValidRight = -1;
                            dirTab[1] = -1;
                        }
                    } else
                        isValidRight = -1;
                    break;
                case 3:
                    if (i < N - 1) {
                        isValidDirect = 1;
                        printf("petit check oklm\n");
                        if (tabV[i + 1][j] != 0) {
                            isValidDirect = 0;
                            isValidDown = -1;
                            dirTab[2] = -1;
                        }
                    } else
                        isValidDown = -1;
                    break;
                case 4:
                    if (j > 0) {
                        isValidDirect = 1;
                        printf("petit check oklm\n");
                        if (tabV[i][j - 1] != 0) {
                            isValidDirect = 0;
                            isValidLeft = -1;
                            dirTab[3] = -1;
                        }
                    } else
                        isValidLeft = -1;
                    break;
                default:
                    break;
            }
        } while (isValidDirect == 0);

        //si on est sorti, peut casser le mur
        if (isValidDirect == 1) {
            switch (dir) {
                case 1:
                    //on casse le mur
                    tabX[i][j] = 1;

                    //on ajoute la matrice d'adjacence
                    matAdj[i][j] = 1 + matAdj[i][j];

                    //inverse de la dir courante
                    lastdir = 4;

                    //on stocke la position courante
                    coordTemp.x = i;
                    coordTemp.y = j;
                    coordTemp.lastDir = matAdj[i][j];

                    printf("%s\n", "----Ecrire dans SHM---");
                    setMatAdj_SHM(sem, matAdj[i][j], i, j, M);

                    printf("adjacence : %d\n", matAdj[i][j]);
                    empiler(maPile, coordTemp);

                    //on se déplace
                    i--;

                    //on incrémente le compteur
                    cpt++;
                    printf("je monte !\n");
                    break;
                case 2:
                    tabY[i][j + 1] = 1;
                    matAdj[i][j] = 2 + matAdj[i][j];

                    //inverse de la dir courante
                    lastdir = 8;
                    coordTemp.x = i;
                    coordTemp.y = j;
                    coordTemp.lastDir = matAdj[i][j];

                    printf("%s\n", "----Ecrire dans SHM---");
                    setMatAdj_SHM(sem, matAdj[i][j], i, j, M);

                    printf("adjacence : %d\n", matAdj[i][j]);
                    empiler(maPile, coordTemp);
                    j++;

                    //on incrémente le compteur
                    cpt++;
                    printf("je vais à droite !\n");
                    break;
                case 3:
                    tabX[i + 1][j] = 1;
                    matAdj[i][j] = 4 + matAdj[i][j];

                    //inverse de la dir courante
                    lastdir = 1;
                    coordTemp.x = i;
                    coordTemp.y = j;
                    coordTemp.lastDir = matAdj[i][j];

                    printf("%s\n", "----Ecrire dans SHM---");
                    setMatAdj_SHM(sem, matAdj[i][j], i, j, M);

                    printf("adjacence : %d\n", matAdj[i][j]);
                    empiler(maPile, coordTemp);
                    i++;

                    //on incrémente le compteur
                    cpt++;
                    printf("je vais en bas !\n");
                    break;
                case 4:
                    tabY[i][j] = 1;
                    matAdj[i][j] = 8 + matAdj[i][j];

                    //inverse de la dir courante
                    lastdir = 2;
                    coordTemp.x = i;
                    coordTemp.y = j;
                    coordTemp.lastDir = matAdj[i][j];

                    printf("%s\n", "----Ecrire dans SHM---");
                    setMatAdj_SHM(sem, matAdj[i][j], i, j, M);

                    printf("adjacence : %d\n", matAdj[i][j]);
                    empiler(maPile, coordTemp);
                    j--;

                    //on incrémente le compteur
                    cpt++;
                    printf("je vais à gauche !\n");
                    break;
                default:
                    break;
            }
            printf("------Fin de l'etape : %d--------\n", cpt);
        }
    }

    //last pos
    printf("derniere position de V=>%d:%d\n", i, j);

    //je visite la last pos
    tabV[i][j] = 1;

    //je lui ajoute son adjacence
    matAdj[i][j] = lastdir;
    setMatAdj_SHM(sem, matAdj[i][j], i, j, M);
    printf("derniere adjacence: %d\n", matAdj[i][j]);

    //On display les matrices pour vérifier le résultat
    printf("%s\n", "Afficher Labyrinthe ?");
    printf("%s\n", "--TabV--");
    for (a = 0; a < M; a++) {
        for (b = 0; b < N; b++) {
            printf("%d", tabV[a][b]);
        }
        printf("\n");
    }

    printf("%s\n", "--TabX--");
    for (a = 0; a < M + 1; a++) {
        for (b = 0; b < N; b++) {
            printf("%d", tabX[a][b]);
        }
        printf("\n");
    }

    printf("%s\n", "--TabY--");
    //system("clear");
    for (a = 0; a < M; a++) {
        for (b = 0; b < N + 1; b++) {
            printf("%d", tabY[a][b]);
        }
        printf("\n");
    }

    printf("%s\n", "--MatAdj--");
    for (a = 0; a < M; a++) {
        for (b = 0; b < N; b++) {
            printf("%d |", matAdj[a][b]);
        }
        printf("\n");
    }
    printf("%s\n", "----Lecture dans SHM----");
    for (a = 0; a < M; a++) {
        for (b = 0; b < N; b++) {
            printf("%d |", getMatAdj_SHM(a, b, M));
        }
        printf("\n");
    }
}

int generateMatAdj_SHM(key_t key) {
    int shm;        // identifiant de la memoire partagee

    // creation du segment de memoire partagee
    if ((shm = shmget(key, LG_CHAINE, IPC_CREAT | 0600)) == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    printf("Genere SHM id: %d\n", shm);

    printf("%s\n", "----Bind matAdj dans SHM---");
    // attachement du segment shm sur le pointeur *chaine
    if ((adjM = shmat(shm, NULL, 0)) == (void *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }   

    return shm;
}

int generateSem(key_t key)
{
    int sem;               // identifiant du semaphore
    union semun u_semun;          // initialisation semaphores
    unsigned short table[1];  // table de valeur du semaphore

  // acces a l'ensemble semaphore (ensemble de 1 semaphore)
  if ((sem = semget(key, 1, 0)) == -1) {
    // creation de l'ensemble qui n'existe pas
    if ((sem = semget(key, 1, IPC_CREAT | IPC_EXCL | 0600)) == -1) {
      perror("semget");
      exit(EXIT_FAILURE);
    }
    // initialisation a vide de la chaine en shm
    adjM[0] = '\0';
    // initialisation de la table de valeur du compteur
    table[0] = 1;
    // initalisation du troisieme membre de l'union
    u_semun.array = table;
    // initialisation de la valeur du compteur dansle semaphore
    if (semctl(sem, 0, SETALL, u_semun) < 0) perror("semctl");
  }

    printf("Genere Sem id: %d\n", sem);
    return sem;
}

void setMatAdj_SHM(int sem ,int value, int row, int column, int rowMax) {
    //entree en section critique / enfin je crois
    P(sem);
    adjM[row * rowMax + column] = value;
    V(sem);
}

int getMatAdj_SHM(int row, int column, int rowMax) {
    //printf("valeur retournee: %d\n", adjM[row * rowMax + column]);
    return adjM[row * rowMax + column];
}

void destroyMatAdj_SHM(int shm) {
    printf("Destruction SHM id: %d\n", shm);
    shmctl(shm, IPC_RMID, NULL);
}

int P(int semId)
{
    struct sembuf buffer;
    buffer . sem_num = 0;
    buffer . sem_op = -1;
    buffer . sem_flg = SEM_UNDO;
    return (semop (semId, & buffer, 1));
}

int V(int semId)
{
    struct sembuf buffer;
    buffer . sem_num = 0;
    buffer . sem_op = 1;
    buffer . sem_flg = SEM_UNDO;
    return (semop (semId, & buffer, 1));
}









