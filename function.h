#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED

/*Definition des fonctions du programme*/
void SetCursorPos(int YPos, int XPos);
void generateMaze(int M, int N);

int generateSHM(int argc, char *argv0,  char *argv1);
void writeSHM(int shm, int * chaine, int value, int row, int column, int rowMax);
void readSHM(int shm, int * chaine, int row, int column, int rowMax);
void destroySHM(int shm);


#endif /* FUNCTION_H_INCLUDED */
