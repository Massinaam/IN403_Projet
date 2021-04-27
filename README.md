# Projet Algorithmique

> Projet IN403 - UVSQ
>
# Sommaire
- [Introduction](#introduction)
- [Canevas](#canevas)
- [Création du graphe](#création-du-graphe)
- [Connexité](#connexité)
- [Recherche du plus court chemin & table de routage](#recherche-du-plus-court-chemin--table-de-routage)
- [Divers](#divers)

## Introduction

### Présentation du problème
Nous souhaitons créer un réseau connexe et non orienté matérialisé par un graphe. Ce dernier est caractérisé par trois types de "noeuds" ou sommets : tier1, tier2 et tier3. Les premiers sont très connectés entre eux, les seconds relativement connectés entre eux et aux premiers, et enfin les tier3 sont plutôt des noeuds de périphérie, peu connectés entre eux mais connectés aux noeuds de tier2 qu'on pourrait qualifier de noeud de transit. Chaque lien entre sommets est pondéré, une valeur respective leur est attitrée représentant le "côut de transit", par exemple en terme de durée. Cela illustre schématiquement le fonctionnement d'un réseau au sens algorithmique, et soulève une question fondamentale : quel-est le chemin optimal pour envoyer une information d'un sommet x vers un sommet y, en terme de "côut de transit" ?

### Pistes de résolution
Pour répondre à cet interrogation avec un programme informatique, il convient de créer un graphe avec les propriétés susdites et réfléchir à une structure de données pour le stocker en mémoire. Ensuite, il faut établir une table de routage pour chaque noeud, il sera nécessaire de choisir un algorithme de plus court chemin parmi ceux que l'on connait pour y parvenir. Il suffit ensuite d'exploiter ces tables de routage pour établir le chemin optimal en terme de coût pour transiter un paquet d'informations d'un noeud x vers un noeud y.

### Choix du langage de programmation
Nous avons chosi de programmer cette application en langage C, nous nous sentons plus à l'aise avec celui-ci d'une part, et d'autre part, c'est un langage réputé pour sa rapidité d'exécution, pas forcément négligeable lorsqu'il s'agit de réaliser beaucoup de calculs et de comparaison. Tous les codes sont mis en fichiers annexes. A noter que les codes que vous verrez ici sont à titre illustratif, ils ne sont pas voués à être testés sur machine. Le groupe décline toute responsabilité en cas de réclamation sur le code à visée pédagogique.

## Canevas
Tout d'abord, il faut créer les fichiers nécessaires au projet : [`Makefile`](https://github.com/Massinaam/IN403_Projet/blob/818456ce6c2355a5d19f8b53cc096493bdb17c32/Makefile), [`graphe.h`](https://github.com/Massinaam/IN403_Projet/blob/818456ce6c2355a5d19f8b53cc096493bdb17c32/graphe.h), [`graphe.c`](https://github.com/Massinaam/IN403_Projet/blob/818456ce6c2355a5d19f8b53cc096493bdb17c32/graphe.c) et [`main.c`](https://github.com/Massinaam/IN403_Projet/blob/818456ce6c2355a5d19f8b53cc096493bdb17c32/main.c). Le code source est librement accessible sur github dans le dossier [IN403_Projet de @Massinaam](https://github.com/Massinaam/IN403_Projet).

## Création du graphe
### *struct* Noeud
La structure de données permettant de stocker le graphe est composée de 5 champs représentant un noeud du graphe. Un nom, un nombre de voisins, une liste chaînée de ceux-ci (voisins), les poids reliant le sommet à ses voisins, mais aussi une table de routage. A partir de cette structure, on définit le graphe comme un ensemble de noeuds, i.e *struct* Noeud* NOEUD.
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
Cette fonction permet la création d'un graphe (I), ici de 100 noeuds, auxquels un "type" (tier1, tier2 ou tier3) est donné de façon à "ordonner" les tier1 de l'indice 0 à 9, les tier2 de 10 à 29 et les tier3 de 30 à 99. 
On nomme ensuite les sommets selon ce raisonnement : le chiffre des centaines indique le type du noeud et les suivants indiquent sa position dans le tableau de noeuds (le graphe) (II).
(III) Enfin, on alloue la mémoire maximale occupée par le tableau de voisins et de pondération pour chaque noeud du graphe et selon son type.
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
### *NOEUD* creationT1
Cette fonction connecte les noeuds tier1 entre eux avec 75% de chances que deux noeuds soient connectés entre eux. Brièvement, on balaye la partie de tableau de noeuds (graphe) correspondant aux tier1 et on ajoute des arrêtes entres eux (probabilité de 0.75), avec pondération p appropriée (4 < p < 11).
```c
NOEUD creationT1(NOEUD graphe, int nbsommets){
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
  }
  return graphe;
}
```

### *NOEUD* creationT2
Cette fonction connecte les noeuds tier2 entre eux et aux noeuds tier1 de façon aléatoire. Un noeud de type tier2 a un ou deux voisin(s) de tier1 et 2, ou alors, 3 voisins de tier2.
Ici, on réalise les mêmes opérations que dans [`creationT1`](#noeud-creationt1), en plus complexe en raison de la spécificité des noeuds de transit. Les arêtes partant d'un tier2 sont pondérées entre 10 et 20.
Concrètement, pour un noeud tier2 considéré, on prend un noeud tier1 et on s'assure qu'il ne pré-existe pas d'arrête entre eux grâce à la fonction [`different`](#noeud-different) pour créer une arête pondérée aléatoirement.
Ensuite, on doit créer les arêtes entre les noeuds tier2. Pour chaque noeud tier2 considéré, on vérifie à l'aide la fonction [`voisin_tiers2`](#int-voisin_tiers2) s'il a atteint le nombre maximal de voisins tier2 (au plus 3) et on s'assure que le noeud considéré n'est pas connecté avec lui-même en créant les arêtes.
```c
NOEUD creationT2(NOEUD graphe,int nbsommets){
for(int i=0; i<nbsommets; i++){
    if(i<30 && i>9)
      int nb_tiers1=(random()%2)+1;		
      for(int j=1; j<=nb_tiers1; j++){
        int tiers1
        ;
        //le do while permet de vérifier qu'on n'ajoute pas 2 fois la même arête
        do {tiers1=random()%nbsommets/10;}
        while(different(graphe[tiers1], graphe[i].suivant, graphe[i].nb_voisin)==0);
        graphe[i].poids[graphe[i].nb_voisin]=(random()%11)+10; //poids de l'arête dans un sens
        graphe[tiers1].poids[graphe[tiers1].nb_voisin]=graphe[i].poids[graphe[i].nb_voisin]; //poids de l'arête dans le sens inverse (graphe non orienté)
        graphe=ajout_arete(i, tiers1,graphe); //ajout de l'arête dans le graphe
        }
    int nb_tiers2=(random()%2)+2;
    nb_tiers2 = nb_tiers2-voisin_tiers2(graphe[i]);
    for(int j=1; j<=nb_tiers2; j++){
        int infini = 0; //détail technique, pour que le graphe ne boucle pas à l'infini (même noeud plusieurs fois)
      	int tiers2=(random()%20)+10;
        while( voisin_tiers2(graphe[tiers2]) >= 3 
             || different(graphe[tiers2], graphe[i].suivant, graphe[i].nb_voisin) == 0 
	     || tiers2 == i ){
        tiers2=(random()%20)+10;
        infini++; 
        if(infini>500){
          printf("Le graphe n'a pas réussi à se construire\n"); //dans le cas où le graphe boucle
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
Cette fonction connecte les noeuds tier3 à deux noeuds tier2 et un autre noeud tier3 de façon aléatoire. Le code est similaire à ce qui à été vu précedemment ([`creationT2`](#noeud-creationT2)) et même raisonnement.

### *NOEUD* implementationGraphe
Afin d'implémenter le graphe, on appelle simplement les fonctions de création d'arêtes pour les différents types de noeuds. On fait ensuite appel à [`table_routage_1`](#noeud-table_routage_1) pour créer les tables de routage pour chaque noeud. Cf. partie [table de routage](#recherche-du-plus-court-chemin--table-de-routage).
```c
NOEUD implementationGraphe(NOEUD graphe,int nbsommets){
  graphe=creationT1(graphe,nbsommets);
  graphe=creationT2(graphe,nbsommets);
  graphe=creationT3(graphe,nbsommets);
  graphe=table_routage_1(graphe,nbsommets);
return graphe;
}
```

### *NOEUD* different
Cette fonction permet de vérifier s'il existe ou non un lien. Elle renvoie 0 si les noeuds sont connectés entre eux, sinon 1.
```c
int different(struct Noeud n1,NOEUD suivant, int nb_voisins){
  int i=0; 
  while(i<nb_voisins){
    if(n1.nom==suivant[i].nom) return 0;
    i++;
  }
  return 1; 
}
```

### *int* voisin_tiers2
Cette fonction retourne le nombre de noeuds voisin(s) de tier2 du `noeud n` entré en argument.

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
Cette fonction permet de vérifier si le `noeud n` entré en argument a un voisin de tier3, auquel cas elle retourne 1, sinon 0. (Même raisonnement de programmation que pour [`voisin_tiers2`](#int-voisin_tiers2).

## Connexité 
### *void* explorer
Cette fonction fait un parcours en profondeur du graphe considéré. Elle est utile pour évaluer la connexité du graphe dans [`marquage`](#int-marquage)
```c
void explorer(struct Noeud s, int T[100], int i){
  T[i]=1; 
  for (int j=0; j<s.nb_voisin; j++){
    i=(s.suivant[j].nom)%100;
    if(T[i]==0) explorer(s.suivant[j],T,i);
  }
}
```
### *int* marquage
Cette fonction permet de marquer les sommets ayant été parcourus et d'explorer ceux qui ne l'ont pas encore été. Elle retourne 1 si le graphe est connexe. On utilise la fonction récursive [`explorer`](#void-explorer).
```c
int marquage(NOEUD G, int nbsommets){ 
  int T[100]; 
  for(int i=0; i< nbsommets; i++) T[i]=0;
  for(int i=0; i< nbsommets; i++){
    if(T[i]==0){
      explorer(G[i], T, i);  //Marque les sommets parcourus, T[i] = 1
    }
  }
 for(int i=0; i<nbsommets; i++){
   if (T[i]==0) return 0;
  }
 return 1;
}
```
## Recherche du plus court chemin & table de routage

### *int* dans_min
Explication 
```c


```
### *int* min_poids
### *int* verif_arete

```c 
int verif_arete(struct Noeud n, int nom){
  int i=0;
  while(i<n.nb_voisin){
    if(n.suivant[i].nom==nom) return i;
    i++;
  }
  return -1;
}
```
### *NOEUD* liste_destinataire
### *void* retrouve_chemin
### *NOEUD* calcul_distance
### *NOEUD* table_routage_1
### *NOEUD* tableRoutage2
### *NOEUD* tableRoutage

## Divers
### *void* libererGraphe
### *void* affiche_noeuds
