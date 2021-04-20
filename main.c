#include "graphe.h"
#include <stdio.h>
#include <stdlib.h>


int main(void) {
  // nombre noeuds du graphe (10 pour essayer car petit nb)
  int nbsommets=100;
  NOEUD graphe = creerGraphe(nbsommets);
	graphe = implementationGraphe(graphe,nbsommets);
	// affiche le graphe selon liste adjacence
	//afficheGraphe(graphe);
  /*int i=10; 
  while(i<30)
  {printf("nombre de voisins de 2%d est %d\n", i, voisin_tiers2(graphe[i]));
  i++;}*/
 marquage(graphe, nbsommets);

 
graphe=tableRoutage(graphe, nbsommets);
 
//affiche_noeuds(graphe,nbsommets);
 
	
	libererGraphe(graphe, nbsommets);
	return 0;
}
