#include "../Netlist.h"
#include "../Init/F_Init.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int entre(double y1, double y2,double y){
	return (y1<=y && y<=y2)||(y2<=y && y<=y1);
}

int intersection(Netlist *N, Segment *s1, Segment *s2){
	if(!s1 || !s2){
		printf("Segment null non comparable\n");
		 return 0;
	}
	
	/* Si ils ont tout les deux la meme orientation ou sont d'un meme reseau, ils ne s'intersectent pas */
	if(s1->HouV==s2->HouV||s1->NumRes==s2->NumRes) return 0;
	
	/* Si s1 est vertical on verifie que l’abscisse de s1 est comprise dans l’intervalle definie par les abscisses de s2 (dans la variable test1) et que l’ordonnee de s2 est comprise dans l’intervalle defini par les ordonnees de s1 (dans la variable test2)*/

	if(s1->HouV) {
		Point *p1s1=N->T_Res[s1->NumRes]->T_Pt[s1->p1], *p2s1=N->T_Res[s1->NumRes]->T_Pt[s1->p2];
		Point *p1s2=N->T_Res[s2->NumRes]->T_Pt[s2->p1], *p2s2=N->T_Res[s2->NumRes]->T_Pt[s2->p2];
		int test1,test2;
		
		test1=entre(p1s2->x,p2s2->x,p1s1->x);
		test2=entre(p1s1->y,p2s1->y,p1s2->y);
		return test1 && test2 ;
	}
	/* Sinon c'est que c'est s2 qui est vertical, on échange s1 et s2 */
	return intersection(N,s2,s1);
}

int nb_seg(Netlist *n){ 
	Cell_segment *tmp;
	Reseau *r;
	int i, j, nbseg=0;
	
	for(j=0;j< n->NbRes;j++){
		r=n->T_Res[j];
		
		for(i=0;i < r->NbPt;i++){
			tmp=r->T_Pt[i]->Lincid;
			
			while(tmp){
				if(tmp->seg->p1 == i) nbseg++;
				tmp=tmp->suiv;
			}
		}
	}
	return nbseg;
}

Segment* *Tab_seg(Netlist *n){
	int nbseg=0,j;	
	Cell_segment *liste=get_seg_from_net(n,&nbseg), *tmp=liste;
	Segment **tabSeg=malloc(nbseg*sizeof(Segment *));
	
	for(j=0;j<nbseg && tmp;j++){
		tabSeg[j]=tmp->seg;
		tmp=tmp->suiv;		
	}
		
	liberer_seglist(liste);
	return tabSeg;
}

void intersect_naif(Netlist *n){
	Segment* *tab=Tab_seg(n);
	int nbseg=nb_seg(n),i,j;
	
	for(i=0;i<nbseg-1;i++)
		for(j=i+1;j<nbseg;j++)
			if( intersection(n,tab[i],tab[j]) ){ 
				tab[i]->Lintersec=insert_seg(tab[j],tab[i]->Lintersec);
				tab[j]->Lintersec=insert_seg(tab[i],tab[j]->Lintersec);
			}
		
	free(tab);
}
	
void Sauvegarde_intersection(Netlist *n, char *filename){
	Segment* *tab=Tab_seg(n);
	int nbseg=nb_seg(n),i;
	Cell_segment *tmp;
	char nomfic[100]="Instance_Intlist/";
	
	strcat(nomfic,strstr(filename,"/"));
	strcat(nomfic,".int");

	FILE *f=fopen(nomfic,"w");
	if(!f){
		printf("Erreur à l'ouverture du fichier en écriture dans Sauvegarde_intersection, fichier: %s\n",nomfic);
		return;
	}

	for(i=0;i<nbseg;i++){
		tmp=tab[i]->Lintersec;
		while(tmp){
			fprintf(f,"%d %d %d %d %d %d\n",tab[i]->NumRes,tab[i]->p1,tab[i]->p2,tmp->seg->NumRes,tmp->seg->p1,tmp->seg->p2);
			tmp=tmp->suiv;
		}
	}
	fclose(f);
	free(tab);
}
		
