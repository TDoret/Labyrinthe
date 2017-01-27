#ifndef H_PILE
#define H_PILE

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

typedef struct Pile Pile;
struct Pile
{
    Element *premier;
};

Pile *initialiser();
void empiler(Pile *pile, Coord coord);
Coord depiler(Pile *pile);
void afficherPile(Pile *pile);

#endif
