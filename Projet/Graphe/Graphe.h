#ifndef _Graphe_H_
#define _Graphe_H_


Sommet *creer_so(char PouS, void *donnee, int indice);


Agregation *inserer_agregation(Agregation *a, Sommet *s);


void liberer_agregation(Agregation *ag);


Sommet* *creer_sommets(const char *ficInt, int *taille, Netlist *N);


Graphe *creer_graphe(const char *ficInt, Netlist *N);


void liberer_graphe(Graphe *g);


int *remplir_S(Graphe *g,int *nbvias);


#endif
