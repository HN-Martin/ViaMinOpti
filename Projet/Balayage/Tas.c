#include "../Netlist.h"
#include <stdlib.h>

int iPere(int ifils){
	return (ifils-1)/2;
}

int iFilsG(int ipere){
	return 2*ipere+1;
}

int iFilsD(int ipere){
	return 2*ipere+2;
}

int hasPere(int ifils){
	return ifils>0;
}

int hasFilsG(int ipere, int taille){
	int indF=iFilsG(ipere);
	if(indF>=taille) return 0;
	return 1;
}


int hasFilsD(int ipere,int taille){
	int indF=iFilsD(ipere);
	if(indF>=taille) return 0;
	return 1;
}


int iMinFils(int ipere, Extremite* *tab, int taille){
	int fg=hasFilsG(ipere,taille), ifilsg=iFilsG(ipere), ifilsd=iFilsD(ipere);
	if(!hasFilsD(ipere,taille)){
		if(!fg) return ipere;
		return ifilsg;
	}
	if(!fg) return ifilsd;
	return (tab[ifilsg]->x < tab[ifilsd]->x || (tab[ifilsg]->x==tab[ifilsd]->x && (tab[ifilsg]->VouGouD==1 || (tab[ifilsg]->VouGouD==0 && tab[ifilsd]->VouGouD==2))) )?ifilsg:ifilsd;
}
	

void echanger(int i1, int i2, Extremite* *tab){
	Extremite *tmp=tab[i1];
	tab[i1]=tab[i2];
	tab[i2]=tmp;
}

void monter(int i, Extremite* *tab, int taille){
	if(!hasPere(i)) return;
	int ipere=iPere(i);
	if(tab[ipere]->x > tab[i]->x || (tab[ipere]->x==tab[i]->x && (tab[i]->VouGouD==1 || (tab[i]->VouGouD==0 && tab[ipere]->VouGouD==2)) )){
		echanger(ipere,i,tab);
		monter(ipere,tab,taille);
	}
}
	
void descendre(int i, Extremite* *tab, int taille){
	int ifils=iMinFils(i,tab,taille);
	if(ifils==i)return;
	if(tab[ifils]->x < tab[i]->x || (tab[ifils]->x==tab[i]->x && (tab[i]->VouGouD==2 || (tab[i]->VouGouD==0 && tab[ifils]->VouGouD==1 ) ))){
		echanger(ifils,i,tab);
		descendre(ifils,tab,taille);
	}
}

void insert(Extremite *e, Extremite* *tab, int taille){
	tab[taille]=e;	
	monter(taille,tab,taille);
}

Extremite* getMin(Extremite* *tab,int taille){
	Extremite *res=tab[0];
	echanger(0,taille-1,tab);
	descendre(0,tab,taille-1);
	return res;
}



