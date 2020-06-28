#ifndef _GRAPHE_CYCLE_
#define _GRAPHE_CYCLE_

Agregation *detecte_cycle_impair(Graphe *g, int *S);


int *ajout_cycle(Agregation **ag, Graphe *g, int *M, int i, int valP);


int ajout_vias_cycle_impair(Graphe *g, int *S);


void bicolore(Graphe *g, int *S);


void colorier(int *S, Graphe *g, int ind, int pere);


int *remplir_S_cycle(Graphe *g,int *nbvias);

#endif
