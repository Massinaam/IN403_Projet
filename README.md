# Projet Algorithmique

> Projet IN403 - UVSQ
>

## Introduction

### Présentation du problème
Nous souhaitons créer un réseau connexe et non orienté matérialisé par un graphe. Ce dernier est caractérisé par trois types de "noeuds" ou sommets : tier1, tier2 et tier3. Les premiers sont très connectés entre eux, les seconds relativement connectés entre eux et aux premiers, et enfin les tier3 sont plutôt des noeuds de périphérie, peu connectés entre eux mais connectés aux noeuds de tier2 qu'on pourrait qualifier de noeud de transit. Chaque lien entre sommets est pondéré, une valeur respective leur est attitrée représentant le "côut de transit", par exemple en terme de durée. Cela illustre schématiquement le fonctionnement d'un réseau au sens algorithmique, et soulève une question fondamentale : quel-est le chemin optimal pour envoyer une information d'un sommet x vers un sommet y, en terme de "côut de transit" ?

### Pistes de résolution
Pour répondre à cet interrogation avec un programme informatique, il convient de créer un graphe avec les propriétés susdites et réfléchir à une structure de données pour le stocker en mémoire. Ensuite, il faut établir une table de routage pour chaque noeud, il sera nécessaire de choisir un algorithme de plus court chemin parmi ceux que l'on connait pour y parvenir. Il suffit ensuite d'exploiter ces tables de routage pour établir le chemin optimal en terme de coût pour transiter un paquet d'informations d'un noeud x vers un noeud y.

### Choix du langage de programmation
Nous avons chosi de programmer cette application en langage C, nous nous sentons plus à l'aise avec celui-ci d'une part, et d'autre part, c'est un langage réputé pour sa rapidité d'exécution, pas forcément négligeable lorsqu'il s'agit de réaliser beaucoup de calculs et de comparaison. Tous les codes sont mis en fichiers annexes. A noter que les codes que vous verrez ici sont à titre illustratif, ils ne sont pas voués à être testés sur machine. Le groupe décline toute responsabilité en cas de réclamation sur le code à visée pédagogique.

## Canevas
Tout d'abord, il faut créer les fichiers nécessaires au projet : `Makefile`, `graphe.h`, `graphe.c` et `main.c`


## Création du graphe
La structure de données permettant de stocker le graphe est composée de 5 champs représentant un noeud du graphe. Un nom, un nombre de voisin, une liste châinée de ceux-ci, les poids reliant le sommet à ses voisins, mais aussi une table de routage. A partir de cette structure, on définit le graphe comme un ensemble de noeuds, i.e *struct* Noeud* NOEUD.

### *struct* Noeud
```c
struct Noeud
{
	int nom;			// nom du noeud considéré (représenté par un entier)
	int nb_voisin; 			// nombre de voisins, représente aussi la position du nouveau noeud à insérer
	struct Noeud* suivant;		// pointe le voisin suivant dans la liste chaînée, le cas échéant
	int* poids;			// ième poids de l'arrête entre le noeud et le ième voisin du noeud
 	int tableRoutage[2][100]; 	// table de routage du noeud
}; 
typedef struct Noeud* NOEUD;		// matérialise le graphe (ensemble de struct Noeud)
```


### *NOEUD* creerGraphe
Cette fonction permet la création d'un graphe (I), ici de 100 noeuds, auxquels un "type" (tier1, tier2 ou tier3) est donné de façon à "ranger" les tier1 de l'indice 0 à 9, les tier2 de l'indice 10 à 29 et les tier 3 de 30 à 99. (II) On les nomment ensuite avec le postulat suivant : le chiffre des centaines indique le type du noeud et les suivants indiquent sa position dans le tableau de noeuds (le graphe).
```c

NOEUD creerGraphe(int nbsommets){
  //----------(I)----------
  NOEUD graphe=malloc(nbsommets*sizeof(struct Noeud));
  // -> vérif malloc
  
  //----------(II)---------
  for(int i=0; i<nbsommets; i++){
    if(i<nbsommets/10) graphe[i].nom=nbsommets+i;
	else if(i<((nbsommets-(nbsommets/10))/3)) graphe[i].nom=nbsommets*2+i;
		else graphe[i].nom = nbsommets*3+i;
    graphe[i].nb_voisin=0;
  }
  
  //---------(III)---------				 
  for(int i=0; i<nbsommets; i++){
    int x; 				// nombre de voisins maximum selon le type de noeud
    if(i<10) x=30;
    	else if(i<30) x=100;
    		else x=3;
    graphe[i].suivant=malloc(sizeof(struct Noeud)*x);
    // -> vérif malloc
    graphe[i].poids=malloc(sizeof(int)*x);
    // -> vérif malloc
  }    
  return graphe;
}
```
(III) Ci-dessus on alloue la mémoire maximale occupée par le tableau de voisins et de pondération pour chaque noeud du graphe et selon son type.

### *NOEUD* creationT1
Cette fonction connecte les noeuds tier1 entre eux avec 75% de chances que deux noeuds soient connectés entre eux. Brièvement, on balaye la partie de tableau de noeuds (graphe) correspondant aux tier1 et on ajoute des arrêtes entres eux (probabilité de 0.75), avec pondération p appropriée (4 < p < 11).

### *NOEUD* creationT2
Cette fonction connecte les noeuds tier2 entre eux et aux noeuds tier1 de façon aléatoire. Un noeud de tier2 à un ou deux voisin(s) de tier1 et 2 ou 3 de tier2. Ici, on réalise les mêmes opérations que dans [`creationT1`](###*NOEUD* creationT1), en plus complexe en raison de la spécificité des noeuds de transit. Les arrêtes partant d'un tier2 sont pondérées entre 10 et 20.
```c
NOEUD creationT2(NOEUD graphe,int nbsommets){
for(int i=0; i<nbsommets; i++){
    if(i<30 && i>9){ 				// Arêtes entre les tiers 2 et les tiers 1
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
      while( voisin_tiers2(graphe[tiers2]) >= 3 
             || different(graphe[tiers2], graphe[i].suivant, graphe[i].nb_voisin) == 0 
	     || tiers2 == i ){
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
  }
  return graphe;
}
```
### *NOEUD* creationT3
Cette fonction connecte les noeuds tier3 à deux noeuds tier2 et un autre noeud tier3 de façon aléatoire. Le code est similaire à ce qui à été vu précedemment, il serait redondant de s'attarder dessus. Le code est disponible en annexe au besoin.

### *NOEUD* implementationGraphe
Explication


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
### *void* explorer
### *int* marquage
### *int* dans_min
### *int* min_poids
### *int* verif_arete
### *NOEUD* liste_destinataire
### *void* retrouve_chemin

## Recherche du plus court chemin & table de routage
### *NOEUD* calcul_distance
### *NOEUD* table_routage_1
### *NOEUD* tableRoutage2
### *NOEUD* tableRoutage

## Divers
### *void* libererGraphe
### *void* affiche_noeuds
