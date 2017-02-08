#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED

#if !defined(__APPLE__)
/* on my mac this is a redefinition */
union semun {
  int val;
  struct semid_ds *buf;
  ushort *array;
};
#endif

/*Definition des fonctions du programme*/
void SetCursorPos(int YPos, int XPos);

void generateMaze(int M, int N, int sem);

int generateMatAdj_SHM(key_t key);

int getMatAdj_SHM(int row, int column, int rowMax);

void setMatAdj_SHM(int sem, int value, int row, int column, int rowMax);

void destroyMatAdj_SHM(int shm);

int generateSem(key_t key);

int P(int semId);

int V(int semId);

#endif /* FUNCTION_H_INCLUDED */
