#include "../Netlist.h"
#include "../Init/F_Init.h"
#include <stdlib.h>
#include <stdio.h>
#include "GrapheCycle.h"

Sommet *creer_so(char PouS, void *donnee, int indice){
	Sommet *s=malloc(sizeof(Sommet));
	s->PouS=PouS;
	s->data=donnee;
	s->liste=NULL;
	s->ind=indice;
	return s;
}
	
Agregation *inserer_agregation(Agregation *a, Sommet *s){
	if(!s) return a;
	Agregation *res=malloc(sizeof(Agregation));
	res->so=s;
	res->suiv=a;
	return res;
}

void liberer_agregation(Agregation *ag){
	Agregation *agtmp;
	while(ag){
		agtmp=ag;
		ag=ag->suiv;
		free(agtmp);
	}
}

Sommet* *creer_sommets(const char *ficInt, int *taille, Netlist *N){
	int iRes, iPt, iCourant=0, iPtGraphe=0;		/* iPtGraphe : nombre de sommet de correspondant à un point dans le graphe */
	Segment *stmp;
	Cell_segment *c_stmp;
	
	/* Calcul de la taille */
	for(iRes=0;iRes<N->NbRes;iRes++)
		for(iPt=0;iPt<N->T_Res[iRes]->NbPt;iPt++){
			(*taille)++;
			for(c_stmp=N->T_Res[iRes]->T_Pt[iPt]->Lincid;c_stmp;c_stmp=c_stmp->suiv)
				if(iPt==c_stmp->seg->p1) (*taille)++;
		}
	
	Sommet* *res=malloc((*taille)*sizeof(Sommet *));
	void *pts;
	
	/* Creation des sommets pour les points */
	for(iRes=0;iRes<N->NbRes;iRes++)
		for(iPt=0;iPt<N->T_Res[iRes]->NbPt;iPt++){
			pts=N->T_Res[iRes]->T_Pt[iPt];
			res[iCourant++]=creer_so('P',pts,iCourant-1);
		}
	
	/* Creation des sommets pour les segments et création des arêtes de C */	
	for(iRes=0;iRes<N->NbRes;iRes++){		
		for(iPt=0;iPt<N->T_Res[iRes]->NbPt;iPt++)		
			for(c_stmp=N->T_Res[iRes]->T_Pt[iPt]->Lincid;c_stmp;c_stmp=c_stmp->suiv){
				if(iPt==c_stmp->seg->p1){
					res[iCourant++]=creer_so('S',c_stmp->seg,iCourant-1);
					res[iPtGraphe+c_stmp->seg->p1]->liste=inserer_agregation(res[iPtGraphe+c_stmp->seg->p1]->liste,res[iCourant-1]);
					res[iPtGraphe+c_stmp->seg->p2]->liste=inserer_agregation(res[iPtGraphe+c_stmp->seg->p2]->liste,res[iCourant-1]);
					res[iCourant-1]->liste=inserer_agregation(res[iCourant-1]->liste,res[iPtGraphe+c_stmp->seg->p2]);
					res[iCourant-1]->liste=inserer_agregation(res[iCourant-1]->liste,res[iPtGraphe+c_stmp->seg->p1]);
				}
				
			}
		iPtGraphe+=N->T_Res[iRes]->NbPt;
	}
	
	FILE *f=fopen(ficInt,"r");
	int numRes1, s1p1, s1p2, numRes2, s2p1, s2p2, iS1, iS2;
	
	/* Creation des arêtes de X */
	while(fscanf(f," %d %d %d %d %d %d",&numRes1,&s1p1,&s1p2,&numRes2,&s2p1,&s2p2)==6){
		for(iCourant=iPtGraphe;iCourant<(*taille);iCourant++){
			stmp=(Segment *)(res[iCourant]->data);
			if(stmp->NumRes==numRes1 && stmp->p1==s1p1 && stmp->p2==s1p2)
				iS1=iCourant;
			if(stmp->NumRes==numRes2 && stmp->p1==s2p1 && stmp->p2==s2p2)
				iS2=iCourant;
			
		}
		res[iS1]->liste=inserer_agregation(res[iS1]->liste,res[iS2]);
	}
	
	fclose(f);
	
	return res;
}

Graphe *creer_graphe(const char *ficInt, Netlist *N){
	int t=0;
	Graphe *g=malloc(sizeof(Graphe));
	g->sommets=creer_sommets(ficInt,&t,N);
	g->taille=t;
	return g;
}
			
void liberer_graphe(Graphe *g){
	int i;
	Agregation *ag, *agtmp;
	
	for(i=0;i<g->taille;i++){
		liberer_agregation(g->sommets[i]->liste);
		free(g->sommets[i]);
	}
	free(g);
}	
			
int *remplir_S(Graphe *g, int *nbvias){
	int i, test=0, t=g->taille, *res=malloc(t*sizeof(int));
	Segment *stmp;
	Agregation *ag;
	*nbvias=0;
	
	for(i=t-1;i>=0;i--){
		if(g->sommets[i]->PouS=='S'){
			stmp=(Segment *)(g->sommets[i]->data);
			res[i]=stmp->HouV+1;
		}else{
			ag=g->sommets[i]->liste;
			test=0;
			while(ag && test!=3){
				stmp=(Segment *)(ag->so->data);
				if(test !=1 && stmp->HouV) test+=1;
				if(test !=2 && !stmp->HouV) test+=2;
				ag=ag->suiv;
			}
			res[i]=test-3;
			if(test==3) (*nbvias)+=1;
		}
	}	
	return res;
}				

