#ifndef _T_H_
#define _T_H_
#include "../Netlist.h"

/* Alloue et initialise l'élément correspondant au segment s en le placant dans la liste trié selon l'ordonnée*/
T *inserer_T(Segment *s, double ord, T *liste);

/* Libère l'élément de la liste contenant le segment s en conservant la continuité de celle ci*/
T *supprimer_T(Segment *s, T *liste);

/* Renvoie le premier élément de la liste dont l'ordonee est supérieure à y*/
T *Prem_segment_apres_T(double y, T *liste);

/*Renvoie l élément de la liste dont l'ordonnee est supérieure et la plus proche de h*/
T *AuDessus_T(T *h, T *liste);

/* Libère la mémoire allouée par T */
void liberer_T(T *liste);

#endif

