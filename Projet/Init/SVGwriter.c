#include "SVGwriter.h"
#include <stdlib.h>
#include "string.h"
#include "F_Init.h"
#include "../Graphe/Graphe.h"
#include "../Graphe/GrapheCycle.h"

void SVGinit(SVGwriter *svg, char *nom, int sizeX, int sizeY) {
  char filename[100];
  strcpy(filename,nom);
  strcat(filename,".html");
  svg->file=fopen(filename, "w");
  
  if (svg->file==NULL) {
    printf("Unable to create %s\n",nom);
    exit(1);
  }
  strcpy(svg->lineColor, Red);
  strcpy(svg->pointColor, Black);

  svg->sizeX=sizeX;
  svg->sizeY=sizeY;
  svg->gencol[0]='0';svg->gencol[1]='1';svg->gencol[2]='2';svg->gencol[3]='3';
  svg->gencol[4]='4';svg->gencol[5]='5';svg->gencol[6]='6';svg->gencol[7]='7';
  svg->gencol[8]='8';svg->gencol[9]='9';svg->gencol[10]='A';svg->gencol[11]='B';
  svg->gencol[12]='C';svg->gencol[13]='D';svg->gencol[14]='E';svg->gencol[15]='F';
  fprintf(svg->file,"<html><body><svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.2\"");
  fprintf(svg->file, " width=\"100%%\" height=\"100%%\"");
  fprintf(svg->file, " viewBox=\"%lf %lf %lf %lf\"", -2.0, -2.0, sizeX+7.0, sizeY+7.0);
  fprintf(svg->file, " preserveAspectRatio=\"yes\">\n");
  fprintf(svg->file, "<g >\n\n");

   svg->lineColor[0]='#';
}


void SVGlineColor(SVGwriter *svg, char *col) {
  strcpy(svg->lineColor,col);
}

void SVGlineRandColor(SVGwriter *svg){
  int i;
  for (i=1;i<=6;i++) svg->lineColor[i]=svg->gencol[rand()%16];

}

void SVGpointColor(SVGwriter *svg, char *col) {
  strcpy(svg->pointColor,col);
}



void SVGpoint(SVGwriter *svg, double x, double y) {
  fprintf(svg->file,"<circle cx=\"%lf\" cy=\"%lf\" r=\"2\" stroke=\"%s\" stroke-width=1\"1\" fill=\"%s\" />\n",x,y,svg->pointColor,svg->pointColor);
}


void SVGline(SVGwriter *svg,double xa,double ya,double xb,double yb) {
  fprintf(svg->file, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" ", xa, ya, xb, yb);
  fprintf(svg->file, " style=\"stroke:%s;stroke-width:1\"/>\n", svg->lineColor);    
}


void SVGfinalize(SVGwriter *svg) {
  fprintf(svg->file, "\n\n</g></svg></body></html>\n");
  fclose(svg->file);
  svg->file=NULL;
}

void SVGdessinerNetlist(Netlist *n, char *dest){
	SVGwriter svg;
	SVGinit(&svg,dest,400,400);
	
	int i, j, nbseg, nbpt;
	Cell_segment *c;
		
	for(i=0;i < n->NbRes;i++){
		
		c=get_seg_from_res(n->T_Res[i],&nbseg);
		nbpt=n->T_Res[i]->NbPt;
		SVGlineRandColor(&svg);
		/* Dessin des points */
		for(j=0;j<nbpt;j++)
			SVGpoint(&svg,n->T_Res[i]->T_Pt[j]->x, n->T_Res[i]->T_Pt[j]->y);
		
		/* Dessin des segments */
		while(c->suiv){
			SVGline(&svg,n->T_Res[i]->T_Pt[c->seg->p1]->x,n->T_Res[i]->T_Pt[c->seg->p1]->y,n->T_Res[i]->T_Pt[c->seg->p2]->x,n->T_Res[i]->T_Pt[c->seg->p2]->y);
			c=c->suiv;
		}
		SVGline(&svg,n->T_Res[i]->T_Pt[c->seg->p1]->x,n->T_Res[i]->T_Pt[c->seg->p1]->y,n->T_Res[i]->T_Pt[c->seg->p2]->x,n->T_Res[i]->T_Pt[c->seg->p2]->y);		
		liberer_seglist(c);
	}
	SVGfinalize(&svg);
}

void SVGdessinerGraphe(Graphe *g, Netlist *N, char *dest){
	SVGwriter svg;
	SVGinit(&svg,dest,400,400);
	Agregation *ag;
	Point *s1p1, *s1p2, *s2p1, *s2p2, *ptmp;
	Segment *stmp;
	int i;
	double x1, x2, y1, y2;	
		
	for(i=0;i<g->taille;i++){
		if(g->sommets[i]->PouS=='S'){
			SVGpointColor(&svg,"3");
			SVGlineColor(&svg,"#AC9DAB");
			stmp=(Segment *)(g->sommets[i]->data);
			s1p1=N->T_Res[stmp->NumRes]->T_Pt[stmp->p1];
			s1p2=N->T_Res[stmp->NumRes]->T_Pt[stmp->p2];
			y1=(s1p1->y + s1p2->y)/2;
			x1=(s1p1->x + s1p2->x)/2;
		}else{
			ptmp=(Point *)(g->sommets[i]->data);
			SVGpointColor(&svg,"1");
			SVGlineColor(&svg,"#76931F");
			x1=ptmp->x;
			y1=ptmp->y;
		}
		SVGpoint(&svg,x1,y1);
					
		ag=g->sommets[i]->liste;	
		while(ag){
			if(ag->so->PouS=='P') goto continuer;	
			stmp=(Segment *)(ag->so->data);
			s2p1=N->T_Res[stmp->NumRes]->T_Pt[stmp->p1];
			s2p2=N->T_Res[stmp->NumRes]->T_Pt[stmp->p2];
			y2=(s2p1->y + s2p2->y)/2;			
			x2=(s2p1->x + s2p2->x)/2;
			SVGline(&svg,x1,y1,x2,y2);
			continuer: ag=ag->suiv;
		}
	}
	SVGfinalize(&svg);
}

void SVGgrapheDeuxFaces(Graphe *g, Netlist *N, char *dest, char cycle){
	SVGwriter svg;
	SVGinit(&svg,dest,400,400);
	Agregation *ag;
	Point *s2p1, *s2p2, *ptmp;
	int i, nbvias, *S=(cycle?remplir_S_cycle(g,&nbvias):remplir_S(g,&nbvias));
	Segment *stmp;
	double x1, x2, y1, y2;
	
	printf("Il y a %d vias dans cette Netlist\n",nbvias);
		
	for(i=0;i<g->taille;i++){
		
		if(g->sommets[i]->PouS=='P'){
			ptmp=(Point *)(g->sommets[i]->data);
			SVGpointColor(&svg,"1");
			x1=ptmp->x;
			y1=ptmp->y;
			if(!S[i]) SVGpoint(&svg,x1,y1);
			SVGlineColor(&svg,"#76931F");
			
			ag=g->sommets[i]->liste;	
			while(ag){
				if(ag->so->PouS=='P') goto continuer;		
				stmp=(Segment *)(ag->so->data);
				s2p1=N->T_Res[stmp->NumRes]->T_Pt[stmp->p1];
				s2p2=N->T_Res[stmp->NumRes]->T_Pt[stmp->p2];
				y2=(s2p1->y + s2p2->y)/2;
				x2=(s2p1->x + s2p2->x)/2;
				if(S[ag->so->ind]==2) SVGlineColor(&svg,"#FF0000");
				SVGline(&svg,x1,y1,x2,y2);
				continuer: ag=ag->suiv;
			}
		}
	}
	SVGfinalize(&svg);
	free(S);	
}


	
