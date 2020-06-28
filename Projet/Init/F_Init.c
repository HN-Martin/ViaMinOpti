#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "../Netlist.h"
#include "F_Init.h"

Cell_segment *insert_seg(Segment *ainserer, Cell_segment *liste){
	Cell_segment *l=malloc(sizeof(Cell_segment));
	l->seg=ainserer;
	l->suiv=liste;
	return l;
}

Cell_segment *copy(Cell_segment *liste){
	Cell_segment *s=NULL,*tmp=liste;
	
	while(tmp){
		s=insert_seg(tmp->seg,s);
		tmp=tmp->suiv;
	}
	return s;
}

Cell_segment *concatenation(Cell_segment *liste1,Cell_segment *liste2){
	if(!liste1) return liste2;
	if(!liste2) return liste1;
	
	Cell_segment *tmp=liste1;
	while(tmp->suiv) tmp=tmp->suiv;
	tmp->suiv=liste2;
	return liste1;
}

Cell_segment *get_seg_from_res(Reseau *r, int *nbseg){ /* *nbseg étant le nombre de segments du reseau */
	Cell_segment *liste=NULL,*tmp;
	int i;
	*nbseg=0;
	for(i=0;i < r->NbPt;i++){
		tmp=r->T_Pt[i]->Lincid;
		while(tmp){
			if(tmp->seg->p1 == i){
				liste=insert_seg(tmp->seg,liste);
				*nbseg=*nbseg+1;
			}
			tmp=tmp->suiv;
		}
	}
	return liste;
}

Cell_segment *get_seg_from_net(Netlist *net, int *nbseg){
	int nbtemp,j;	/* *nbtemp sert à compter le nombre de segment pour un reseau par effet de bord de get_seg_from_res	*/
	*nbseg=0;
	Cell_segment *liste=NULL;
	Reseau *r;
	
	for(j=0;j< net->NbRes;j++){
		r=net->T_Res[j];		
		liste=concatenation(liste,get_seg_from_res(r,&nbtemp));
		*nbseg+=nbtemp;
	}
	return liste;
}	
	
Segment *creer_seg(int p1, int p2, int NumRes, Netlist *n){
	Segment *s=malloc(sizeof(Segment));
	s->p1=p1;
	s->p2=p2;
	s->NumRes=NumRes;
	s->HouV=(n->T_Res[NumRes]->T_Pt[p1]->x == n->T_Res[NumRes]->T_Pt[p2]->x)?1:0;
	s->Lintersec=NULL;
	
	/* On place le segment dans les Lincid de ses deux points */
	
	n->T_Res[NumRes]->T_Pt[p1]->Lincid = insert_seg(s,n->T_Res[NumRes]->T_Pt[p1]->Lincid);
	n->T_Res[NumRes]->T_Pt[p2]->Lincid = insert_seg(s,n->T_Res[NumRes]->T_Pt[p2]->Lincid);
	
	return s;
}

Point *creer_point(double x, double y, int NumRes, Netlist *n){
	Point *p = malloc(sizeof(Point));
	p->x=x;
	p->y=y;
	p->NumRes=NumRes;
	p->Lincid=NULL;
	/* On place le point dans le tableau de son reseau */
	n->T_Res[NumRes]->T_Pt[ (n->T_Res[NumRes]->NbPt) ]= p;
	n->T_Res[NumRes]->NbPt++;
	
	return p;
}


Reseau *creer_res(const int maxPts, Netlist *n){
	Reseau *r=malloc(sizeof(Reseau));
	
	r->NbPt=0;		
	r->NumRes=n->NbRes;
	n->NbRes++;
	r->T_Pt = (Point **) malloc(maxPts*sizeof(Point *));
	/* On place le reseau dans le tableau de sa netlist */
	n->T_Res[r->NumRes]=r;
	
	return r;
}

Netlist *creer_net(const int maxRes){
	Netlist *n=malloc(sizeof(Netlist));
	n->NbRes=0;
	n->T_Res = (Reseau **) malloc(maxRes*sizeof(Reseau *));
	return n;
}


Netlist *lectureNetlist(const char *nomfic){
	FILE *f=fopen(nomfic,"r");
	if(!f /*|| !strstr(nomfic,".net")*/){
		printf("Erreur à l'ouverture du fichier\n");
		return NULL;
	}
	int lec1, lec2, lec3, nRes, nPts, nSeg, numRes; /* lec comme lecture car utilisés dans les fscanf */
	
	fscanf(f,"%d",&lec1);
	
	Netlist *n=creer_net(lec1);
	nRes=lec1;
	
	while(nRes){	/* Lecture des Reseaux */
		if(fscanf(f," %d %d %d",&lec1,&lec2,&lec3)!=3){
			printf("Erreur à la lecture d'un reseau\n");
			continue;
		}
		numRes=lec1;
		nPts=lec2;
		nSeg=lec3;
				
		n->T_Res[numRes]=creer_res(lec2,n);
		
		while(nPts){ 	/* Lecture des points */
			if(fscanf(f," %d %d %d",&lec1,&lec2,&lec3)!=3){
				printf("Erreur à la lecture d'un point\n %d %d %d\n",lec1,lec2,lec3);
				continue;
			}
			creer_point((double)lec2,(double)lec3,numRes,n);
			nPts--;
		}
		while(nSeg){	/* Lecture des segments */
			if(fscanf(f," %d %d",&lec1,&lec2)!=2){
				printf("Erreur à la lecture d'un segment\n");
				continue;
			}
			creer_seg(lec1,lec2,numRes,n);
			nSeg--;
		}
		nRes--;
	}
	fclose(f);
	return n;
}

void ecritureNetlist(const char *nomfic, Netlist *n){
	FILE *f=fopen(nomfic,"w");
	if(!f || !n ){
		printf("Erreur à l'ouverture du fichier\n");
		return;
	}
	int i, j, nbseg, nbpt;
	Cell_segment *c;
	
	fprintf(f,"%d\n",n->NbRes);
	
	for(i=0;i < n->NbRes;i++){
		c=get_seg_from_res(n->T_Res[i],&nbseg);
		nbpt=n->T_Res[i]->NbPt;
		fprintf(f,"%d %d %d\n",i,nbpt,nbseg);
		
		/* Ecriture des points */
		for(j=0;j<nbpt;j++)
			fprintf(f,"  %d %f %f\n",j,n->T_Res[i]->T_Pt[j]->x, n->T_Res[i]->T_Pt[j]->y);
		
		/* Ecriture des segments */
		while(c->suiv){
			fprintf(f,"  %d %d\n",c->seg->p1,c->seg->p2);
			c=c->suiv;
		}
		fprintf(f,"  %d %d\n",c->seg->p1,c->seg->p2);
		
		liberer_seglist(c);
	}
	fclose(f);
}
			
						

void liberer_netlist(Netlist *n){
	int i;
	for(i=0;i < n->NbRes;i++)
		liberer_reseau(n->T_Res[i]);
	free(n);
}

void liberer_reseau(Reseau *r){
	Cell_segment *liste=NULL,*tmp;
	int i;
	for(i=0;i<r->NbPt;i++){
		tmp=r->T_Pt[i]->Lincid;
		while(tmp){
			if(tmp->seg->p1 == i){
				liste=insert_seg(tmp->seg,liste);
			}
			tmp=tmp->suiv;
		}
		liberer_point(r->T_Pt[i]);
	}
	liberer_seglist_et_segments(liste);		
}

void liberer_point(Point *p){
	liberer_seglist(p->Lincid);
	free(p);}		

void liberer_seg(Segment *s){
	free(s);
}

void liberer_seglist_et_segments(Cell_segment *liste){
	Cell_segment *tmp;
	while(liste){
		tmp=liste;
		liste=liste->suiv;
		liberer_seg(tmp->seg);
		free(tmp);
	}
}

void liberer_seglist(Cell_segment *liste){
	Cell_segment *tmp;
	while(liste){
		tmp=liste;
		liste=liste->suiv;
		free(tmp);
	}
}
