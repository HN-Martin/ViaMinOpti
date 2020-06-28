#include <stdio.h>
#include <stdlib.h>
#include "../Netlist.h"
#include "../Init/F_Init.h"
#include "Tas.h"
#include "AVL.h"
#include "T.h"

Extremite *creer_ext(Point *p, Segment *s, Netlist *net){
	Extremite *ext=malloc(sizeof(Extremite));
	ext->x=p->x;
	
	Point *minP1P2=net->T_Res[s->NumRes]->T_Pt[s->p2];
	int num=s->p2;
	
	if(net->T_Res[s->NumRes]->T_Pt[s->p1]->x < net->T_Res[s->NumRes]->T_Pt[s->p2]->x){
		minP1P2=net->T_Res[s->NumRes]->T_Pt[s->p1];
		num=s->p1;
	}
	
	ext->VouGouD=s->HouV?0:(minP1P2==p?1:2);
	ext->PtrSeg=s;
	ext->NumPt=num;
	return ext;
}



Extremite* *creer_echeancier(Netlist *net, int *taille){
	int iRes, iPt, iCourant=0;
	Cell_segment *tmp;	
	*taille=0;
	
	for(iRes=0;iRes < net->NbRes; iRes++)
		for(iPt=0;iPt < net->T_Res[iRes]->NbPt;iPt++)
			for(tmp=net->T_Res[iRes]->T_Pt[iPt]->Lincid;tmp;tmp=tmp->suiv)
				if(iPt==tmp->seg->p1){
					if(!tmp->seg->HouV)	*taille= (*taille) +1;
				 	*taille= (*taille) +1;
				}
	
	Extremite* *ec=malloc((*taille)*sizeof(Extremite *));
	
	/* Insertion sous forme de tas */
	for(iRes=0;iRes < net->NbRes; iRes++)
		for(iPt=0;iPt < net->T_Res[iRes]->NbPt;iPt++)
			for(tmp=net->T_Res[iRes]->T_Pt[iPt]->Lincid;tmp;tmp=tmp->suiv)
				 if(iPt==tmp->seg->p1){	
				 	if(!tmp->seg->HouV)
				 		insert(creer_ext(net->T_Res[iRes]->T_Pt[tmp->seg->p2],tmp->seg,net),ec,iCourant++);
				 	insert(creer_ext(net->T_Res[iRes]->T_Pt[tmp->seg->p1],tmp->seg,net),ec,iCourant++);
				 }
	
	/* Tri par tas en O( nlog(n) ) */
	
	for(;iCourant>0;iCourant--) ec[iCourant-1]=getMin(ec,iCourant);	
	
	return ec;
}

Extremite* *tri_echeancier(Extremite* *ec, int taille){ /*	Tri par tas de l'échéancier en O( nlog(n) )	*/
	if(!ec || !taille) return ec;
	int nbelem;
	
	for(nbelem=0;nbelem<taille;nbelem++)
		insert(ec[nbelem],ec,nbelem);
		
	for(;nbelem>=0;nbelem--)
		ec[taille-nbelem]=getMin(ec,nbelem);
	
	return ec;
}
	
void liberer_echeancier(Extremite* *ec, int taille){
	int i;
	for(i=0;i<taille;i++)
		free(ec[i]);
	free(ec);
}

void intersec_balayage_T(Extremite* *tab, int taille, Netlist *N){
	int i;
	double y1, y2,y;
	T *liste=NULL,*h;
	
	for(i=taille-1;i>=0;i--){
		y1=N->T_Res[tab[i]->PtrSeg->NumRes]->T_Pt[tab[i]->PtrSeg->p1]->y;
		printf("i : %d, VouGouD=%d\n",i,tab[i]->VouGouD);
		switch(tab[i]->VouGouD){
			case 1: liste=inserer_T(tab[i]->PtrSeg,y1,liste);
				break;
			case 2: liste=supprimer_T(tab[i]->PtrSeg,liste);
				break;
			case 0: y2=N->T_Res[tab[i]->PtrSeg->NumRes]->T_Pt[tab[i]->PtrSeg->p2]->y;
				if(y2<y1){
					y=y1;
					y1=y2;
					y2=y;
				}
				for(h=Prem_segment_apres_T(y1,liste);h && h->y < y2;h=AuDessus_T(h,liste)){
					if(h->seg->NumRes == tab[i]->PtrSeg->NumRes) continue;
					tab[i]->PtrSeg->Lintersec=insert_seg(h->seg,tab[i]->PtrSeg->Lintersec);
					h->seg->Lintersec=insert_seg(tab[i]->PtrSeg,h->seg->Lintersec);
				}
				break;
			default: printf("VouGouD mal initialisé\n");
		}
	}
	liberer_T(liste);
}

void intersec_balayage_AVL(Extremite* *tab, int taille, Netlist *N){
	int i;
	double y1,y2,y;
	AVLseg *avl=NULL,*h;

	for(i=taille-1;i>=0;i--){
		printf("\ni : %d, VouGouD=%d\n",i,tab[i]->VouGouD);
		y1=N->T_Res[tab[i]->PtrSeg->NumRes]->T_Pt[tab[i]->PtrSeg->p1]->y;
		switch(tab[i]->VouGouD){
			case 1: printf("avm: %ld, y: %f\n",avl,y1);
				avl=insert_AVL(avl,tab[i]->PtrSeg,y1);
				break;
			case 2: printf("avm: %ld, y: %f\n",avl,y1);
				avl=supprimer_Noeud(avl,tab[i]->PtrSeg,y1);
				break;
			case 0: y2=N->T_Res[tab[i]->PtrSeg->NumRes]->T_Pt[tab[i]->PtrSeg->p2]->y;
				if(y2<y1){
					y=y1;
					y1=y2;
					y2=y;
				}
				for(h=Prem_segment_apres_AVL(y1,avl);avl && h->y < y2;h=AuDessus_AVL(h,avl)){
					if(h->seg->NumRes == tab[i]->PtrSeg->NumRes) continue;
					tab[i]->PtrSeg->Lintersec=insert_seg(h->seg,tab[i]->PtrSeg->Lintersec);
					h->seg->Lintersec=insert_seg(tab[i]->PtrSeg,h->seg->Lintersec);
				}
				break;
			default: printf("VouGouD mal initialisé\n");
		}
		afficher_AVL(avl);
	}
	liberer_AVL(avl);
}

