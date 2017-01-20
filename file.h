#ifndef H_FILE
#define H_FILE
typedef struct Coord Coord;
struct Coord
{
	int x;
	int y;
};

typedef struct Element Element;
struct Element
{
    Coord coord;
    Element *suivant;
};

typedef struct File File;
struct File
{
    Element *premier;
};

File *initialiser();
void enfiler(File *file, Coord coord);
Coord defiler(File *file);
void afficherFile(File *file);

#endif
