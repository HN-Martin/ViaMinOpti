#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../Netlist.h"
#include "../Init/F_Init.h"
#include "Tas.h"
#include "AVL.h"
#include "T.h"
#include "F_Balayage.h"

int main(int argc,char *argv[]){
	if(argc!=2){
		printf("Indiquez un fichier .net en parametre\n");
		return 1;
	}
	FILE *f=fopen("TestBalayage.data","a");
	Netlist *net=lectureNetlist(argv[1]);
	int taille, i=2;
	Extremite* *ec=creer_echeancier(net, &taille);
	
	
	/*do{	
		printf("Quelle structure souhaitez-vous utiliser ? 1: Liste , 2: AVL\n");
		scanf("%d",&i);
	}while(i<1 || i>2);
	*/
	clock_t t1, t0=clock();
	
	if(i==1) intersec_balayage_T(ec,taille,net);
	else intersec_balayage_AVL(ec,taille,net);
	
	t1=clock();
	fprintf(f,"%d\t%lf\n",taille,(double)(t1-t0));
	
	liberer_echeancier(ec,taille);
	
	liberer_netlist(net);
	fclose(f);
	return 0;
}
