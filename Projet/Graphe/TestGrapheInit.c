#include "../Init/F_Init.h"
#include <stdlib.h>
#include <time.h>
#include "Graphe.h"
#include "GrapheCycle.h"
#include "../Init/SVGwriter.h"


int main(int argc,char *argv[]){
	if(argc!=4){
		printf("Indiquez un fichier .net en parametre suivi d'un fichier .int et de 0 ou 1 selon si vous préférez utiliser la détection par cycle ou non\n");
		return 1;
	}
	Netlist *net=lectureNetlist(argv[1]);
	Graphe *g=creer_graphe(argv[2],net);
	
	SVGdessinerGraphe(g,net,"Dessin_graphe");
	SVGgrapheDeuxFaces(g,net,"DessinVias",*argv[3]);
	

	liberer_netlist(net);
	liberer_graphe(g);
	return 0;
}
