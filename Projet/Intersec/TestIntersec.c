#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../Init/F_Init.h"
#include "../Netlist.h"
#include "F_Intersec.h"

int main(int argc, char *argv[]){
	if(argc!=2){
		printf("Indiquez un fichier .net en parametre\n");
		return 1;
	}
	FILE *f=fopen("TestIntersec.data","a");
	Netlist *net=lectureNetlist(argv[1]);
	
	clock_t t0,t1;
	t0=clock();
	intersect_naif(net);
	t1=clock();
	fprintf(f,"%d\t%d\n",nb_seg(net),(int)(t1-t0));
	Sauvegarde_intersection(net,argv[1]);
	liberer_netlist(net);
	fclose(f);
	return 0;
}
