#ifndef _F_INTERSEC_H_
#define _F_INTERSEC_H_
#include "../Netlist.h"
/* Renvoie 1 si n est compris entre n1 et n1 */
int entre(double n1,double n2,double n);

/* Renvoie 1 si le segment s1 intersecte s2, 0 sinon. s1 et s2 appartiennent à la Netslist N */
int intersection(Netlist *N, Segment *s1, Segment *s2);

/* Renvoie le nombre de segments présents dans la Netlist n */
int nb_seg(Netlist *n);

/* Renvoie un tableau contenant les pointeurs de chacun des segments de la Netlist n */
Segment* *Tab_seg(Netlist *n);

/* Trouve et ajoute les segments de la Netlist N en intersection entre eux et les place dans leur liste Lintersec */
void intersect_naif(Netlist *n);

/* Sauvegarde dans un fichier filename.int la liste des intersections de la Netlist n */
void Sauvegarde_intersection(Netlist *n, char *filename);

#endif
