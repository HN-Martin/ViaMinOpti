#ifndef _F_BALAYAGE_
#define _F_BALAYAGE_
#include "../Netlist.h"

/* Alloue une extrémité de segment s et de point p et initialise ses champs selon p et s */
Extremite *creer_ext(Point *p, Segment *s, Netlist *net);

/* Alloue et initialise le tableau contenant des pointeurs vers toutes les extrémitées de la Netlist net triées par abscisses décroissantes, la taille de ce tableau est donnée par effet de bord via "taille" */
Extremite* *creer_echeancier(Netlist *net, int *taille);

/* Tri le tableau de pointeurs d'extrémitées ec de taille "taille" selon l'abscisse décroissante */
Extremite* *tri_echeancier(Extremite* *ec, int taille);

/* Trouve et ajoute les segments du tableau de pointeurs d'extremité tab en intersection entre eux et les place dans leur liste Lintersec en utilisant une structure de liste triée de segments horizontaux triée par leur ordonnée */
void intersec_balayage_T(Extremite* *tab,int taille, Netlist *N);

/* Trouve et ajoute les segments du tableau de pointeurs d'extremité tab en intersection entre eux et les place dans leur liste Lintersec en utilisant une structure d'AVL de segments horizontaux triée par leur ordonnée */
void intersec_balayage_AVL(Extremite* *tab,int taille, Netlist *N);

/* Libère la mémoire allouée par l'echeancier */
void liberer_echeancier(Extremite* *ec, int taille);

#endif
