#include "SVGwriter.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "../Netlist.h"
#include "F_Init.h"

int main(int argc, char *argv[]){
	if(argc!=2){
		printf("Indiquez un fichier .net en parametre\n");
		return 1;
	}
	char nomfic[]="Instance_Netlist/ecritureNetlist.net";
	Netlist *net=lectureNetlist(argv[1]);
	//ecritureNetlist(nomfic,net);
	SVGdessinerNetlist(net,"dessinNetlist");
	liberer_netlist(net);
	
	return 0;
}
