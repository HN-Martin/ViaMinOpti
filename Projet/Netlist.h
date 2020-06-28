#ifndef _NETLIST_H_
#define _NETLIST_H_

struct segment;

typedef struct cell_segment{
	struct segment* seg;
	struct cell_segment *suiv;
} Cell_segment;


typedef struct segment{
	int NumRes;	/* Numero du reseau auquel appartient ce segment*/
	int p1, p2;
	int HouV;	/* 0:Horizontal	1:Vertical */
	Cell_segment *Lintersec;
} Segment;

typedef struct point{
	double x, y;	/* Coordonees du point */
	int NumRes;	/* Numero du reseau auquel appartient ce point*/
	Cell_segment   *Lincid;
} Point;

typedef struct reseau{
	int NumRes;	/* Numero du reseau */
	int NbPt;	/* Nombre de points du reseau */
	Point* *T_Pt;
} Reseau;

typedef struct netlist{
	int NbRes;
	Reseau* *T_Res;
} Netlist;

typedef struct s_extremite{
	double x;	/* Abscisse du point */

	int VouGouD;	 /* 0 si segment V / 1 si pt gauche d’un segment H / 2 si pt droit d’un segment H */

	Segment * PtrSeg; /* Pointeur sur le segment correspondant a l’extremite */

	int NumPt; 	/* si segment H: numero du point correspondant */
} Extremite;

typedef struct listeSegH{
	Segment *seg;
	double y;		/* ordonnée du segment */
	struct listeSegH *suiv;
} T;

struct segment_horizontauxAVL;

typedef struct segment_horizontauxAVL{
	Segment *seg;
	struct  segment_horizontauxAVL *fg, *fd;
	int h;
	double y;	/* h est la hauteur de l'arbre et y l'ordonnee du segment horizontal seg */
} AVLseg;

struct liste_sommet;

typedef struct _sommet{
	char PouS;		/* Vaut 'P' pour un point ou 'S' pour un sommet' */
	void *data;		/* Pointeur sur un Point ou un Segment */
	int ind;		/* Indice du sommet dans le tableau de sommets de son graphe */
	struct liste_sommet *liste;
} Sommet;

typedef struct liste_sommet{
	Sommet *so;
	struct liste_sommet *suiv;
} Agregation;

typedef struct _graphe{
	int taille;
	Sommet* *sommets;	/* Tableau des sommets du graphe. Dans les faits, ce tableau est trié à son initialisation de manière à ce que tout les sommets correspondant à des points soient placés avant ceux correspondant à des segments. Cette propriété sera utilisée dans les fonctions */
} Graphe;

#endif
