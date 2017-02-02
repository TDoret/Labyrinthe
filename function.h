#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED

/*Definition des fonctions du programme*/
void SetCursorPos(int YPos, int XPos);
void generateMaze(int M, int N, int shm);

int generateMatAdj_SHM(int argc, char *argv0,  char *argv1);
void bindMatAdj_SHM(int shm);
void setMatAdj_SHM(int value, int row, int column, int rowMax);
int getMatAdj_SHM(int shm, int row, int column, int rowMax);
void bindIntTabInSHM(int shm);
void destroyMatAdj_SHM(int shm);

#endif /* FUNCTION_H_INCLUDED */
