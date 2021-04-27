#include "graphe.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(void) {
  clock_t begin = clock();
  // nombre noeuds du graphe (10 pour essayer car petit nb)
  NOEUD graphe;
  int nbsommets=100;
  int connexe = 1; 
  while(connexe){
    graphe = creerGraphe(nbsommets);
	  graphe = implementationGraphe(graphe,nbsommets);
	  // affiche le graphe selon liste adjacence
	  //afficheGraphe(graphe);
    if(marquage(graphe, nbsommets)){
      printf("La construction du graphe est un succes, le graphe est connexe\n");
      connexe = 0;
    }
    else{
      printf("La construction du graphe est un echec, le graphe n'est pas connexe\n");
      libererGraphe(graphe, nbsommets);
    }
  }
 
  graphe=tableRoutage(graphe, nbsommets);
 
  //affiche_noeuds(graphe,nbsommets);

  retrouve_chemin(graphe,nbsommets);


  libererGraphe(graphe, nbsommets);




  clock_t end = clock();
  unsigned long millis = (end -  begin) * 1000/ CLOCKS_PER_SEC;
  printf("Temps d'execution = %lu ms\n", millis);
  return 0;
}
