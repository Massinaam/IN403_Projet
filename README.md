# Projet Algorithmique

> Projet IN403 - UVSQ
>

## Introduction

### Présentation du problème
Nous souhaitons créer un réseau connexe et non orienté matérialisé par un graphe. Ce dernier est caractérisé par trois types de "noeuds" ou sommets : tier1, tier2 et tier3. Les premiers sont très connectés entre eux, les seconds relativement connectés entre eux et aux premiers, et enfin les tier3 sont plutôt des noeuds de périphérie, peu connectés entre eux mais connectés aux noeuds de tier2 qu'on pourrait qualifier de noeud de transit. Chaque lien entre sommets est pondéré, une valeur respective leur est attitrée représentant le "côut de transit", par exemple en terme de durée. Cela illustre schématiquement le fonctionnement d'un réseau au sens algorithmique, et soulève une question fondamentale : quel-est le chemin optimal pour envoyer une information d'un sommet x vers un sommet y, en terme de "côut de transit" ?

### Pistes de résolution
Pour répondre à cet interrogation avec un programme informatique, il convient de créer un graphe avec les propriétés susdites et réfléchir à une structure de données pour le stocker en mémoire. Ensuite, il faut établir une table de routage pour chaque noeud, il sera nécessaire de choisir un algorithme de plus court chemin parmi ceux que l'on connait pour y parvenir. Il suffit ensuite d'exploiter ces tables de routage pour établir le chemin optimal en terme de coût pour transiter un paquet d'informations d'un noeud x vers un noeud y.

### Choix du langage de programmation
Nous avons chosi de programmer cette application en langage C, nous nous sentons plus à l'aise avec celui-ci d'une part, et d'autre part, c'est un langage réputé pour sa rapidité d'exécution, pas forcément négligeable lorsqu'il s'agit de réaliser beaucoup de calculs et de comparaison. Tous les codes sont mis en fichiers annexes.
 Tous les codes sont mis en fichiers annexes.

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
Explication
```c
// Création des arêtes
NOEUD implementationGraphe(NOEUD graphe,int nbsommets){
  srandom(getpid());
  for(int i=0; i<nbsommets; i++){
    if(i<nbsommets/10){
	    for(int j=0; j<nbsommets/10; j++){
		    if(i!=j){
	        if(different(graphe[j],graphe[i].suivant, graphe[i].nb_voisin)){
			      int alea=random()%4;
			      if(alea<=2){
				      graphe[i].poids[graphe[i].nb_voisin]=(random()%6)+5;
				      graphe[j].poids[graphe[j].nb_voisin]=graphe[i].poids[graphe[i].nb_voisin];
				      graphe=ajout_arete(i, j, graphe);
            }
          }		
		    }
      }
    }
    else if(i<30 && i>9){ 
    // Création des arêtes pour les tiers 2
    // Arêtes entre les tiers 2 et les tiers 1
      int nb_tiers1=(random()%2)+1;
      for(int j=1; j<=nb_tiers1; j++){
        int tiers1;
        do {tiers1=random()%nbsommets/10;}
        while(different(graphe[tiers1], graphe[i].suivant, graphe[i].nb_voisin)==0); 
        graphe[i].poids[graphe[i].nb_voisin]=(random()%11)+10; 
        graphe[tiers1].poids[graphe[tiers1].nb_voisin]=graphe[i].poids[graphe[i].nb_voisin];
        graphe=ajout_arete(i, tiers1, graphe);
      }
    int nb_tiers2=(random()%2)+2;
    nb_tiers2 = nb_tiers2-voisin_tiers2(graphe[i]);
    for(int j=1; j<=nb_tiers2; j++){
      int infini = 0;
	    int tiers2=(random()%20)+10;
      while(voisin_tiers2(graphe[tiers2])>=3 || different(graphe[tiers2], graphe[i].suivant, graphe[i].nb_voisin)==0 || tiers2==i){
        tiers2=(random()%20)+10;
        infini++; 
        if(infini>500){
          printf("Le graphe n'a pas réussi à se construire\n");
          exit(EXIT_FAILURE);
        }
      }
      graphe[i].poids[graphe[i].nb_voisin]=(random()%11)+10;
      graphe[tiers2].poids[graphe[tiers2].nb_voisin]=graphe[i].poids[graphe[i].nb_voisin];
      graphe=ajout_arete(i, tiers2,graphe);
	  }
  }
  else{
    // création sommets de Tier3
    for(int j=1; j<=2; j++){ 
      int tiers2;
      do { tiers2=(random()%(nbsommets/5))+10;}
      while(different(graphe[tiers2], graphe[i].suivant, graphe[i].nb_voisin)==0);
      graphe[i].poids[graphe[i].nb_voisin]=(random()%36)+15; 
      graphe[tiers2].poids[graphe[tiers2].nb_voisin]=graphe[i].poids[graphe[i].nb_voisin];
      graphe=ajout_arete(i, tiers2, graphe); 
    }
    if(arete_tiers3(graphe, nbsommets)==0){   
      if(voisin_tiers3(graphe[i])==0) {
        int tiers3=0; 
        do{tiers3=random()%70+30;}
        while(different(graphe[tiers3], graphe[i].suivant, graphe[i].nb_voisin)==0 || tiers3==i || voisin_tiers3(graphe[tiers3])==1); 
        graphe[i].poids[graphe[i].nb_voisin]=(random()%36)+15;
        graphe[tiers3].poids[graphe[tiers3].nb_voisin]=graphe[i].poids[graphe[i].nb_voisin];
        graphe=ajout_arete(i,tiers3,graphe);
      }
    }
  }
  }
graphe=table_routage_1(graphe,nbsommets);
  return graphe;
}

```

### *NOEUD* different
Explication
```c
// Vérifie qu'un noeud n'est pas déja lié à un autre noeud
int different(struct Noeud n1,NOEUD suivant, int nb_voisins){
  int i=0; 
  while(i<nb_voisins){
    if(n1.nom==suivant[i].nom) return 0;
    i++;
  }
  return 1; 
}
```

### *int* voisin_tiers3
Explication 
```c

int voisin_tiers3(struct Noeud n){
  int i=0; int nb_tiers3_voisin=0;
  while(i<n.nb_voisin){
    if(n.suivant[i].nom>=330 && n.suivant[i].nom<400){
      nb_tiers3_voisin++;
    }
    i++; 
    if (nb_tiers3_voisin==1) return 1;
  }
  return 0;
}
```

### *int* voisin_tiers2
```c
int voisin_tiers2(struct Noeud n){
  int i=0; int nb_tiers2_voisin =0;
  while(i<n.nb_voisin){
    if((n.suivant[i].nom>=210) && (n.suivant[i].nom<230)) {
      nb_tiers2_voisin++;
    }
    i++;
  }
  return nb_tiers2_voisin;
}
```

### *int* voisin_tiers3
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
