# Projet Algorithmique

> Projet IN403 - UVSQ
>

## Introduction
Ce README sert à expliquer le code du projet. Le langage C a été choisi pour ce projet pour des raisons de commodité. Tous les codes sont mis en fichiers annexes.

## Canevas
Tout d'abord, il faut créer les fichiers nécessaires au projet : `Makefile`, `graphe.h`, `graphe.c` et `main.c`


## Création du graphe
Explication

### *struct* noeud
```c
struct Noeud
{
	int nom;//nom du noeud considéré
	int nb_voisin; // nombre de voisins, représente aussi la position du nouveau noeud à insérer
	struct Noeud* suivant;// pointe le voisin suivant dans la liste chaînée le cas échéant
	int* poids;// ième poids de l'arrête entre le noeud et le ième voisin du noeud
  int tableRoutage[2][100]; // table de routage du noeud
};typedef struct Noeud* NOEUD;
```


### *NOEUD* creer graphe
Explication 
```c

NOEUD creerGraphe(int nbsommets){
  // Tableau de struct noeud définissant le graphe
  NOEUD graphe=malloc(nbsommets*sizeof(struct Noeud));
  if(graphe==NULL){
    printf("Problème d'allocation mémoire lors de la création du graphe");
    exit(EXIT_FAILURE);
  }
  ```
  Explication
  ```c }
  //Allocation de la mémoire du tableau de voisins et de pondération selon le type de noeud (typage arbitraire, initialisé ici)
  for(int i=0; i<nbsommets; i++){
    int x; 
    if(i<10) x=T1;
    else if(i<30) x=T2;
    else x=T3;
    graphe[i].suivant=malloc(sizeof(struct Noeud)*x); 
  ```
  
   Explication
  
  
 ```c
   // On vérifie si l'allocation mémoire s'est bien effectuée
    if(graphe[i].suivant==NULL){
      printf("Problème d'allocation mémoire lors de la 		 	 création du tableau de voisins");
      exit(EXIT_FAILURE);
    } 
   ```
   Explication
  ```c
   graphe[i].poids=malloc(sizeof(int)*x);
    if(graphe[i].poids==NULL){
      printf("Problème d'allocation mémoire lors de la création du tableau de pondération des arêtes");
      exit(EXIT_FAILURE);
    } 
  }    
  return graphe;
}
```



### *NOEUD* implementationGraphe

### *int* voisin_tiers2
### *void* libererGraphe
### *void* affiche_noeuds
### *void* explorer
### *int* marquage
### *int* dans_min
### *int* min_poids
### *int* verif_arete
### *NOEUD* liste_destinataire
### *NOEUD* calcul_distance
### *NOEUD* table_routage_1
### *NOEUD* tableRoutage2
### *NOEUD* tableRoutage
### *void* retrouve_chemin
