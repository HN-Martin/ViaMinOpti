#ifndef _F_INIT_H_
#define _F_INIT_H_
#include "../Netlist.h"

/* Insère le segment "ainserer" dans la liste "liste" et retourne la liste obtenue */
Cell_segment *insert_seg(Segment *ainserer, Cell_segment *liste);

/* Copie la liste "liste" et la renvoie */
Cell_segment *copy(Cell_segment *liste);

/* Concatene les listes liste1 et liste2, liste2 est placée après liste1 */
Cell_segment *concatenation(Cell_segment *liste1,Cell_segment *liste2);

/* Retourne une liste contenant les segments du reseau r, la taille de cette liste est obtenue par effet de bord dans *nbseg */
Cell_segment *get_seg_from_res(Reseau *r, int *nbseg);

/* Retourne la liste de tout les segments de la Netslist n, la taille de cette liste est obtenue par effet de bord dans *nbseg */
Cell_segment *get_seg_from_net(Netlist *net, int *nbseg);

/* Alloue un segment, lui affecte les valeurs en paramètre, initialise HouV et l'ajoute aux listes Lincid de ses 2 points */
Segment * creer_seg(int p1, int p2, int numRes, Netlist *n);

/* Alloue un point, lui affecte les valeurs en paramètre, le place dans T_Point[] de son reseau */
Point * creer_point(double x, double y, int numRes, Netlist *n);

/* Alloue un reseau, lui affecte les valeurs en paramètre, le place dans T_Res[] de sa Netlist */
Reseau * creer_res(const int maxPts, Netlist *n);

/* Alloue une Netlist avec maxRes comme nombre maximal de reseaux */
Netlist *creer_net(const int maxRes);

/* Renvoie un pointeur sur la Netlist créée d'après sa description dans le fichier nomfic */
Netlist *lectureNetlist(const char *nomfic);

/* Sauvegarde la Netlist n dnas le fichier nomfic au format .net */
void ecritureNetlist(const char *nomfic, Netlist *n);

/* Libère toute la mémoire allouée par la Netlist n et ses composants */
void liberer_netlist(Netlist *n);

/* Libère toute la mémoire allouée par le reseau r et ses composants */
void liberer_reseau(Reseau *r);

/* Libère toute la mémoire allouée par le point p et ses composants */
void liberer_point(Point *p);

/* Libère toute la mémoire allouée par le segment s et ses composants */
void liberer_seg(Segment *s);

/* Libère toute la mémoire allouée par la liste de segments "liste" et les segments qui la composent */
void liberer_seglist_et_segments(Cell_segment *liste);

/* Libère la mémoire allouée par la liste "liste" sans libérer les segments qui la composent */
void liberer_seglist(Cell_segment *liste);
#endif
