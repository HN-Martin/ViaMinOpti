#include "../Netlist.h"
#include <stdlib.h>
#include <stdio.h>

AVLseg *creer_AVL(Segment *s, double ord){
	AVLseg *res=malloc(sizeof(AVLseg));
	res->seg=s;
	res->h=0;
	res->y=ord;
	res->fg=NULL;
	res->fd=NULL;
	return res;
}

void liberer_AVL(AVLseg *av){
	if(av){
		liberer_AVL(av->fg);
		liberer_AVL(av->fd);
		free(av);
	}
}

int hauteur(AVLseg *av){
	if(!av) return -1;
	return av->h;
}

int maxHauteurFils(AVLseg *av){
	if(!av) return -2;
	int hFilsG=hauteur(av->fg), hFilsD=hauteur(av->fd);
	return hFilsD>hFilsG ? hFilsD:hFilsG;
}

void maj_hauteur(AVLseg *av){
	if(!av) return;
	av->h= 1 + maxHauteurFils(av);
}

AVLseg *rotationD(AVLseg *av){
	if(!av || !av->fg) return av;
	AVLseg *tmp;
	tmp=av->fg;
	av->fg=av->fg->fd;
	maj_hauteur(av);
	tmp->fd=av;
	maj_hauteur(tmp);
	return tmp;
}

AVLseg *rotationG(AVLseg *av){
	if(!av || !av->fd) return av;
	AVLseg *tmp;
	tmp=av->fd;
	av->fd=av->fd->fg;
	maj_hauteur(av);
	tmp->fg=av;
	maj_hauteur(tmp);
	return tmp;
}


AVLseg *insert_ABR(AVLseg *av, Segment *s, double ord){
	if(!av) return creer_AVL(s,ord);
	AVLseg *tmp=av;
	while(tmp){
		if(tmp->y < ord){
			tmp->h+=(maxHauteurFils(tmp)==hauteur(tmp->fd)?1:0);
			if(!tmp->fd){
				tmp->fd=creer_AVL(s,ord);
				return av;
			}
			tmp=tmp->fd;
		}else{
			tmp->h+=(maxHauteurFils(tmp)==hauteur(tmp->fg)?1:0);
			if(!tmp->fg){
				tmp->fg=creer_AVL(s,ord);
				return av;
			}
			tmp=tmp->fg;
		}
	}
	return av;	
}
	
AVLseg *insert_AVL(AVLseg *av,Segment *s, double ord){
	av=insert_ABR(av,s,ord);
	
	printf(" case : %d - %d\n",hauteur(av->fg),hauteur(av->fd));
	switch(hauteur(av->fg)-hauteur(av->fd)){
		case -2:if(hauteur(av->fd->fd)<hauteur(av->fd->fg)) rotationD(av->fd);
			av=rotationG(av);
			break;
		case 2: if(hauteur(av->fg->fg)<hauteur(av->fg->fd)) rotationG(av->fg);
			av=rotationD(av);
			break;
		default: printf("Bien équilibré\n");
	}
	return av;
}

void afficher_AVL(AVLseg *av){
	if(!av) return;
	afficher_AVL(av->fg);
	printf("\t%.1f | %d ",av->y,av->h);
	afficher_AVL(av->fd);
}


void supprimer_D(AVLseg *av){
	AVLseg *p_min=av->fd, *min;
		
	if(!p_min->fg){
		av->fd=p_min->fd;
		av->y=p_min->y;
		av->seg=p_min->seg;
		free(p_min);
		av->h-=1;
		return;
	}
	
	while(p_min->fg->fg){
		if(hauteur(p_min->fg)>hauteur(p_min->fd)) p_min->h-=1;
		p_min=p_min->fg;
	}
	
	p_min->h-=1;
	min=p_min->fg;
	p_min->fg=min->fd;
	av->y=min->y;
	av->seg=min->seg;
	free(min);
}

void supprimer_G(AVLseg *av){
	AVLseg *p_max=av->fg, *max;
		
	if(!p_max->fd){
		av->fg=p_max->fg;
		av->y=p_max->y;
		av->seg=p_max->seg;
		free(p_max);
		maj_hauteur(av);
		return;
	}
	
	while(p_max->fd->fd){
		if(hauteur(p_max->fd)>hauteur(p_max->fg)) p_max->h-=1;
		p_max=p_max->fd;
	}
	
	p_max->h-=1;
	max=p_max->fd;
	p_max->fd=max->fg;
	av->y=max->y;
	av->seg=max->seg;
	free(max);
}
	
		
AVLseg *supprimer_Noeud(AVLseg *av, Segment *s,double ord){
	AVLseg *tmp=av;
	
	printf("%.1f à supprimer \n",ord);
	while(tmp && tmp->seg!=s){
		if(tmp->y <ord){
			if(hauteur(tmp->fd)>hauteur(tmp->fg))tmp->h-=1;
			tmp=tmp->fd;
		}else{
			if(hauteur(tmp->fd)<hauteur(tmp->fg))tmp->h-=1;
			tmp=tmp->fg;
		}
	}
	if(!tmp) goto EssayonsLeGoTo;
	
	if(!(tmp->fd || tmp->fg)){
		free(tmp);
		goto EssayonsLeGoTo;
	}
	if(hauteur(av->fd)>hauteur(av->fg)) supprimer_D(tmp);
	else supprimer_G(tmp);
	
	switch(hauteur(av->fg)-hauteur(av->fd)){
		case -2:if(hauteur(av->fd->fd)<hauteur(av->fd->fg)) rotationD(av->fd);
			av=rotationG(av);
			break;
		case 2: if(hauteur(av->fg->fg)<hauteur(av->fg->fd)) rotationG(av->fg);
			av=rotationD(av);
			break;
		default: printf("Bien équilibré lors de la supression\n");
	}
	
	EssayonsLeGoTo: return av;
}
		
AVLseg *Prem_segment_apres_AVL(double ord, AVLseg *av){
	if(!av) return NULL;
	if(av->y < ord){
		if(av->fd)return Prem_segment_apres_AVL(ord, av->fd);
		return NULL;
	}
	AVLseg *tmp=av, *res;
	
	while(tmp){
		if(tmp->y >ord){
			res=tmp;
			tmp=tmp->fg;
		} else tmp=tmp->fd;
	}
	return res;
}

AVLseg *AuDessus_AVL(AVLseg *h, AVLseg *racine){
	return Prem_segment_apres_AVL(h->y,racine);
}
	
	
	
	


