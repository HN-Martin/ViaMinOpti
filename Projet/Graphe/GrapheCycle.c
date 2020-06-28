#include "../Netlist.h"
#include "../Init/F_Init.h"
#include <stdlib.h>
#include <stdio.h>
#include "Graphe.h"

int ajout_cycle(Agregation **ag, Graphe *g, int *M, int i, int valP){
	if(M[i]==valP) return i+2;
	if(M[i]>=0) return 0;
	if(valP==1) M[i]=2;
	else M[i]=1;
	
	Agregation *tmp=g->sommets[i]->liste;
	int res;
	while(tmp){
		res=ajout_cycle(ag,g,M,tmp->so->ind,M[i]);
		if(res){
			if(res>1 && res-2!=i){
				(*ag)=inserer_agregation(*ag, g->sommets[i]);
				return res;
			}
			if(res-2==i) (*ag)=inserer_agregation(*ag, g->sommets[i]);
			return 1;
		}
		tmp=tmp->suiv;
	}
	return 0;
}

Agregation *detecte_cycle_impair(Graphe *g, int *S){
	const int t=g->taille;
	int i,j, M[t];
	Agregation *res=NULL;
		
	for(i=0;i<t;i++){
		for(j=0;j<t;j++){
			if(S[j]==0) M[j]=0;
			else M[j]=-1;
		}
		if(ajout_cycle(&res,g,M,i,2)) return res;
	}
	return NULL;
}	


int ajout_vias_cycle_impair(Graphe *g, int *S){
	Agregation *agTmp=detecte_cycle_impair(g,S), *aPremierP;
	int nbvias=0;
	
	while(agTmp){
		aPremierP=agTmp;
		while(aPremierP->so->PouS!='P') aPremierP=aPremierP->suiv; 
		/* Il y a forcement un sommet point dans un cycle impair*/
		S[aPremierP->so->ind]=0;
		liberer_agregation(agTmp);
		nbvias+=1;
		agTmp=detecte_cycle_impair(g,S);
	}
	
	return nbvias;
}

void colorier(int *S, Graphe *g, int ind, int pere){
	if(S[ind]>=0) return;
	
	if(pere==1) S[ind]=2;
	else S[ind]=1;
		
	Agregation *ag;
	ag=g->sommets[ind]->liste;
	while(ag){
		colorier(S,g,ag->so->ind,S[ind]);
		ag=ag->suiv;
	}
}
	
void bicolore(Graphe *g, int *S){
	int i;
	
	/* Coloration des segments */
	for(i=0;i<g->taille;i++){
		if(S[i]>=0) continue;
		colorier(S,g,i,2);				
	}
	/* Coloration des points */
	for(i=0;i < g->taille && g->sommets[i]->PouS=='P';i++){
		if(S[i]==0) continue;		
		S[i]=S[g->sommets[i]->liste->so->ind];
	}			
}
	
	
int *remplir_S_cycle(Graphe *g,int *nbvias){
	int i, *res=malloc(g->taille*sizeof(int));
	
	for(i=0;i<g->taille;i++) res[i]=-1;
	
	*nbvias=ajout_vias_cycle_impair(g,res);
	bicolore(g,res);
	return res;
}

