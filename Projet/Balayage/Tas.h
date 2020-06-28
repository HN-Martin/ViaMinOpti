#ifndef _TAS_
#define _TAS_
#include "../Netlist.h"

/* Renvoie l'indice du noeud pere  */
int iPere(int ifils);

/* Renvoie l'indice du noeud fils gauche */
int iFilsG(int ipere);

/* Renvoie l'indice du noeud fils droit */
int iFilsD(int ipere);

/* Renvoie 1 si le noeud d'indice ifils a un pere dans le tas tab, 0 sinon */
int hasPere(int ifils);

/* Renvoie 1 si le noeud d'indice ipere a un fils gauche dans le tas tab, 0 sinon */
int hasFilsG(int ipere, int taille);

/* Renvoie 1 si le noeud d'indice ipere a un fils droit dans le tas tab, 0 sinon */
int hasFilsD(int ipere, int taille);

/* Renvoie l'indice du fils du noeud d'indice ipere ayant la plus petite abscisse ou ipere si il correspond à une feuille */
int iMinFils(int ipere, Extremite* *tab, int taille);

/* Echange le contenu des cases de tab d'indice i1 et i2*/
void echanger(int i1, int i2, Extremite* *tab);

/* Fait monter le noeud d'indice i jusqu'à ce que son noeud pere soit plus petit que lui */
void monter(int i, Extremite* *tab, int taille);

/* Fait descendre le noeud d'indice i jusqu'à ce que aucun de ses fils ne soit plus petit que lui */
void descendre(int i, Extremite* *tab, int taille);

/* Place l'extremite e dans le tas d'extremite tab de taille "taille" */
void insert(Extremite *e, Extremite* *tab, int taille);

/* Retourne le minimum du tas tab de taille "taille" en l'enlevant de ce dernier */
Extremite* getMin(Extremite* *tab,int taille);

#endif
