#include "../Netlist.h"
#include <stdio.h>
#include <stdlib.h>

T *inserer_T(Segment *s, double ord, T *liste){
	if(!s) return liste;
	
	T *res = malloc(sizeof(T));
	res->seg=s;
	res->y=ord;
	res->suiv=NULL;
	
	if(!liste) return res;

	if(liste->y>ord){
		res->suiv=liste;
		return res;
	}

	T *tmp;
	for(tmp=liste; tmp->suiv && tmp->suiv->y < ord ;tmp=tmp->suiv);
	
	if(tmp->suiv && tmp->suiv->suiv) res->suiv=tmp->suiv;
	tmp->suiv = res;
	return liste;
}

T *supprimer_T(Segment *s, T *liste){
	T *tmp=liste, *corbeille;
	if(tmp->seg==s){
		corbeille=tmp->suiv;
		free(tmp);
		return corbeille;
	}	
	while(tmp && tmp->suiv){	
		if(tmp->suiv->seg==s){
			corbeille=tmp->suiv;
			tmp->suiv=tmp->suiv->suiv;
			free(corbeille);
			break;
		}
		tmp=tmp->suiv;
		
	}
	return liste;
}
	
T *Prem_segment_apres_T(double y,T *liste){
	if(!liste) return NULL;
	if(liste->y >=y ) return liste;
	return Prem_segment_apres_T(y,liste->suiv);
}	
	
T *AuDessus_T(T *h,T *liste){
	return h->suiv;
}

void liberer_T(T *liste){
	if(!liste) return;
	T *tmp=liste->suiv;
	free(liste);
	liberer_T(tmp);
}
