#ifndef _AVL_H_
#define _AVL_H_
#include "../Netlist.h"

/* Alloue le noeud d'AVL correspondant au segment s d'ordonnee y */
AVLseg *creer_AVL(Segment *s, double y);

/* Libère la mémoire allouée par l'AVL av sans liberer ses segments */
void liberer_AVL(AVLseg *av);

/* Effectue la rotation droite de la racine de l'AVL av */
AVLseg *rotationD(AVLseg *av);

/* Effectue la rotation gauche de la racine de l'AVL av */
AVLseg *rotationG(AVLseg *av);

/* Retourne la hauteur de l'ABR av */
int hauteur(AVLseg *av);

/* Met à jour la hauteur de tout les noeuds de l'ABR av */
void maj_hauteur(AVLseg *av);

/* Alloue et place le noeud correspondant au segment s (d'ordonnée ord) dans l'AVL av sans garder l'équilibre de av */
AVLseg *insert_ABR(AVLseg *av, Segment *s, double ord);

/* Alloue et place le noeud correspondant au segment s (d'ordonnée ord) dans l'AVL av en conservant l'équilibre de av */
AVLseg *insert_AVL(AVLseg *av, Segment *s, double ord);

/* Affiche le parcours prefixe de l'AVL de racine av */
void afficher_AVL(AVLseg *av);

/* Retourne le noeud pere du noeud d'ordonnee minimum de l'AVL av */
AVLseg *pere_du_max_AVL(AVLseg *av);

/* Retourne le noeud pere du noeud d'ordonnee maximum de l'AVL av */
AVLseg *pere_du_min_AVL(AVLseg *av);

/* Supprime le noeud av en le remplacant par le minimum de son fils droit */
void supprimer_D(AVLseg *av);

/* Supprime le noeud av en le remplacant par le maximum de son fils gauche */
void supprimer_G(AVLseg *av);

/* Supprime le noeud contenant le segment horizontal s d'ordonnee ord dans l'AVL av */
AVLseg *supprimer_Noeud(AVLseg *av, Segment *s,double ord);

/* Renvoie le segment d'un noeud de l'AVL dont l'ordonee est supérieure à y et la plus proche de y*/
AVLseg *Prem_segment_apres_AVL(double ord, AVLseg *av);

/*Renvoie le segment d'un noeud de l'AVL dont l'ordonnee est supérieure et la plus proche de h*/
AVLseg *AuDessus_AVL(AVLseg *h, AVLseg *racine);

#endif
