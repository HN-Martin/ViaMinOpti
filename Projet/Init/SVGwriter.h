#ifndef _SVGWRITER_H_
#define _SVGWRITER_H_
/* Dans cette librairie, les couleurs sont utilisees sous leur forme
   hexadecimale comme le montre les exemples suivants) */

#define Black "#000000"
#define Cyan "#00FFFF"
#define Fuchsia "#FF00FF"
#define Lime "#00FF00"
#define Orange "#FFA800"
#define Red "#FF0000"
#define RoyalBlue "#486CE8"
#include <stdio.h>
#include "../Netlist.h"

typedef struct SVGwriter_struct {
  
  char lineColor[10];  /* Default color value */
  char pointColor[10]; /* Default color value */
  int sizeX, sizeY;    /* Picture size */
   
  FILE *file; /* File stream */

  /* Technical value for random color generation */
  char gencol[16];

} SVGwriter;


  
/* Initialise le struct SVGwriter passe en parametre
      nomfic: nom du fichier html \'a creer
      sizeX, sizeY: taille en pixel de l'image \'a creer */
void SVGinit(SVGwriter *svg,char *nomfic, int sizeX, int sizeY);

/* Fixe la couleur des lignes pour la suite */
void SVGlineColor(SVGwriter *svg, char *col);

/* Fixe une couleur aleatoire pour les lignes pour la suite */
void SVGlineRandColor(SVGwriter *svg);

/* Fixe la couleur des points pour la suite */
void SVGpointColor(SVGwriter *svg, char *col);

/* Ecrit un point dans le fichier */ 
void SVGpoint(SVGwriter *svg, double x, double y);

/*Ecrit un segment dans le fichier */
void SVGline(SVGwriter *svg, double xa, double ya, double xb, double yb);

/* Ferme le fichier */
void SVGfinalize(SVGwriter *svg);

/* Crée un fichier NVG de nom dest représentant une netlist N */
void SVGdessinerNetlist(Netlist *n, char *dest);

/* Crée un fichier NVG de nom dest représentant un graphe G qui correspond à une netlist N */
void SVGdessinerGraphe(Graphe *g, Netlist *N, char *dest);

/* Crée un fichier NVG de nom dest représentant un graphe G qui correspond à une netlist N avec uniquement les points des vias. Les vias sont construit via la méthode de détection par cycle impair si le paramètre cycle est positif. Affiche également sur la sortie standart le nombre de vias */
void SVGgrapheDeuxFaces(Graphe *g, Netlist *N, char *dest, char cycle);

#endif
