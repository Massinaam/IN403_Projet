#include "graphe.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* -----------------------------------------------
> @Authors : Khadija Ben Ayed, Noé Demange, Romain Villa, Massina Ammad
> version du 20/04/2021
--------------------------------------------------*/

//Taille max des tableaux de voisins d'un noeud (et in-extenso de pondération) pour chaque type de noeud
#define T1 30 //(nbsommets/10+nbsommets/5)
#define T2 100 //(nbsommets*3/100+nbsommets/25)
#define T3 3 //(nbsommets/100+nbsommets/50)

// Création d'un graphe en fonction du nombre de sommets
NOEUD creerGraphe(int nbsommets){
  // Tableau de struct noeud définissant le graphe
  NOEUD graphe=malloc(nbsommets*sizeof(struct Noeud));
  if(graphe==NULL){
    printf("Problème d'allocation mémoire lors de la création du graphe");
    exit(EXIT_FAILURE);
  }
  //Initialisation du nom des noeuds et du nombre de leurs voisins à 0.
  for(int i=0; i<nbsommets; i++){
		if(i<nbsommets/10) graphe[i].nom=nbsommets+i;
		else if(i<((nbsommets-(nbsommets/10))/3)) graphe[i].nom=nbsommets*2+i;
		else graphe[i].nom = nbsommets*3+i;
    graphe[i].nb_voisin=0;
  }
  //Allocation de la mémoire du tableau de voisins et de pondération selon le type de noeud (typage arbitraire, initialisé ici)
  for(int i=0; i<nbsommets; i++){
    int x; 
    if(i<10) x=T1;
    else if(i<30) x=T2;
    else x=T3;
    graphe[i].suivant=malloc(sizeof(struct Noeud)*x); 
    // On vérifie si l'allocation mémoire s'est bien effectuée
    if(graphe[i].suivant==NULL){
      printf("Problème d'allocation mémoire lors de la 		 	 création du tableau de voisins");
      exit(EXIT_FAILURE);
    } 
    graphe[i].poids=malloc(sizeof(int)*x);
    if(graphe[i].poids==NULL){
      printf("Problème d'allocation mémoire lors de la création du tableau de pondération des arêtes");
      exit(EXIT_FAILURE);
    } 
  }    
  return graphe;
}


// Vérifie qu'un noeud n'est pas déja lié à un autre noeud
int different(struct Noeud n1,NOEUD suivant, int nb_voisins){
  int i=0; 
  while(i<nb_voisins){
    if(n1.nom==suivant[i].nom) return 0;
    i++;
  }
  return 1; 
}

// Vérifie qu'un noeud Tier2 n'a pas plus de 3 voisins de même type
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

//Vérifie qu'un noeud Tier3 n'a qu'un seul voisin de même type
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

int arete_tiers3(NOEUD graphe, int nbsommet){
  int i=30; 
  while(i<nbsommet){
    if(voisin_tiers3(graphe[i])==0) return 0;
    i++;
  }
  return 1; 
}

// Ajout des arêtes dans les 2 sens (graphe non orienté)
NOEUD ajout_arete(int i,int j, NOEUD graphe){
  // Ajout de l'arête (noeud i, noeud j) dans le tableau de voisins du noeud i
  graphe[i].suivant[graphe[i].nb_voisin]=graphe[j]; 
  graphe[i].nb_voisin++;
 // Ajout de l'arête (noeud j, noeud i) dans le tableau de voisins du noeud j
  graphe[j].suivant[graphe[j].nb_voisin]=graphe[i];
  graphe[j].nb_voisin++;
  return graphe;
}

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

NOEUD creationT2(NOEUD graphe,int nbsommets){
for(int i=0; i<nbsommets; i++){
    if(i<30 && i>9){ 
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
  }
  return graphe;
}

NOEUD creationT3(NOEUD graphe, int nbsommets){
  for(int i=0; i<nbsommets; i++){
  if(i>29){
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
  return graphe;
}
// Création des arêtes
NOEUD implementationGraphe(NOEUD graphe,int nbsommets){
  graphe=creationT1(graphe,nbsommets);
  graphe=creationT2(graphe,nbsommets);
  graphe=creationT3(graphe,nbsommets);
  graphe=table_routage_1(graphe,nbsommets);
return graphe;
}

// Permet la libération de la mémoire occupée par les structures de données du graphe
void libererGraphe(NOEUD graphe, int nbsommets){
  for (int i=0; i<nbsommets; i++){
    free(graphe[i].suivant); 
    free(graphe[i].poids);
  }
  free(graphe);
}


void explorer(struct Noeud s, int T[100], int i){
  T[i]=1; 
  for (int j=0; j<s.nb_voisin; j++){
    i=(s.suivant[j].nom)%100;
    if(T[i]==0) explorer(s.suivant[j],T,i);
  }
}

int marquage(NOEUD G, int nbsommets){ 
  int T[100]; 
  for(int i=0; i< nbsommets; i++) T[i]=0;
  for(int i=0; i< nbsommets; i++){
    if(T[i]==0){
      explorer(G[i], T, i);
    }
  }
  //vérification --> 
 for(int i=0; i<nbsommets; i++){if (T[i]==0) return 0;}
 return 1;
}

NOEUD liste_destinataire(NOEUD graphe, int nbsommets){
  int i=0;
  int j; 
  for(j=0;j<nbsommets;j++){
    graphe[j].tableRoutage[i][j]=graphe[j].nom;  
  } 
  return graphe;
}

int verif_arete(struct Noeud n, int nom){
  int i=0;
  while(i<n.nb_voisin){
    if(n.suivant[i].nom==nom) return i;
    i++;
    //printf("on tourne toujours a %d", i);
  }
  return -1;
}

int dans_min(int min[100], int j, int min_tab){
  for(int i=0; i<min_tab; i++){
    if(j==min[i]%100) return 1;
  }
  return 0;
}

int min_poids(int d[100], int min[100], int min_tab, int nbsommets){
  int j=0; 
  while(d[j]==0 ||dans_min(min,j,min_tab)==1 ) j++;
  //printf("d %d ",j);
  //les problèmes commencent ici dans la boucle qui suit 
  int mini=d[j] ;
  int ind=j;
  //printf("j %d\n",j);
  for(int i=j+1; i<nbsommets; i++)
  {
    if(mini>d[i] && d[i]!=0 && dans_min(min,i, min_tab)==0){mini=d[i]; ind=i;}
  }
// printf("le poid min se trouve a l'indice %d\n", ind);
  return ind;
}

struct Noeud calcul_distance(NOEUD graphe, int nbsommets, struct Noeud n)
{
  int d[100];
  int pred[100];int min[100];int min_tab=0;
  for(int i=0; i<nbsommets; i++){
    if(graphe[i].nom!=n.nom){
      int a=verif_arete(n,graphe[i].nom);
      //printf(" ca marche %d", i);
      if(a!=-1) {d[i]=n.poids[a]; pred[i]=n.nom; }
      else d[i]=2147483647;
    } 
    else {d[i]=0; pred[i]=n.nom; }
    //l'initialisation se fait bien  
    //printf("on est passé par le noeud %d",graphe[i].nom);
  }
  min[min_tab]=n.nom;
  min_tab++;
  //le prob est dans la boucle qui suit
  int nb_rep; 
  for(nb_rep=0; nb_rep<nbsommets-1; nb_rep++) {
     int sommet=min_poids(d,min,min_tab,nbsommets);
      min[min_tab]=graphe[sommet].nom; min_tab++;
      for(int j=0; j<nbsommets; j++){
        if(j!=n.nom%100 && j!=sommet){
          int b=verif_arete(graphe[sommet], graphe[j].nom);
          if(b!=-1 && d[j]>d[sommet]+graphe[sommet].poids[b] && d[sommet]!=2147483647){
            d[j]=d[sommet]+graphe[sommet].poids[b];
            pred[j]=graphe[sommet].nom;
          }
        }
      }
  } 
  n=tableRoutage2(graphe, n,nbsommets,d, pred); 
  /*for(int i=0; i<nbsommets; i++){
    printf("la case %d de min est %d\n", i, min[i]);
    printf("le poid de %d à %d est %d son pred est %d", n.nom,graphe[i].nom, d[i], pred[i]);
    getchar();
  }*/
  return n;
}


void affiche_noeuds(NOEUD n, int nbsommets){
	for (int i = 0; i<nbsommets; i++)
  {
		printf("noeud : %d, nombre(s) de voisin(s) : %d \n", n[i].nom, n[i].nb_voisin);
		if(n[i].nb_voisin !=0)
    {
		  for(int j = 0; j<n[i].nb_voisin;j++)
      {
			printf("arête vers %d, poids %d \n", n[i].suivant[j].nom, n[i].poids[j]);
	  	}
    }
		getchar();
	}
}

NOEUD table_routage_1(NOEUD graphe, int nbsommets){
for(int i=0; i<nbsommets; i++){
  for(int j=0; j<nbsommets; j++){
    graphe[i].tableRoutage[0][j]=graphe[j].nom;
  }
}
return graphe;
}

struct Noeud tableRoutage2(NOEUD graphe, struct Noeud n, int nbsommets, int d[100], int pred[100])
{
  for(int i = 0; i < nbsommets; i++)
  {
   if(n.nom%100!=i){
    if(pred[i]==n.nom%100)
    {
      n.tableRoutage[1][i]=n.tableRoutage[0][i];
    }
    else
    { int position = i;
   // printf("%d et son pred est %d\n",position, pred[position]);
     while(verif_arete(n,graphe[position].nom)==-1)
      {//printf("%d et son pred est %d\n",position, pred[position]);
        position=pred[position]%100;
      }
      n.tableRoutage[1][i]=graphe[position].nom;
    }}
  }
  /*printf("en partant du noeud %d\n", n.nom);
  for (int i=0; i<nbsommets; i++){
    printf("pour arriver a %d, on passe par %d\n", n.tableRoutage[0][i],n.tableRoutage[1][i]);
  } 
  */
  return n;
}

NOEUD tableRoutage(NOEUD graphe, int nbsommets)
{
  for(int i=0; i<nbsommets; i++)
  graphe[i]=calcul_distance(graphe, nbsommets,graphe[i]);
  return graphe;
}

void purge_stdinbuf()
{
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

// fct de saisie noeud utilisateur
// en mode d'exécution 0 -> saisie du noeud émetteur
// en mode d'exécution 1 -> saisie du noeud destinataire
int saisie_noeud(int mode)
{
  if(mode==0)
  {
    int noeud_emet;
    scanf("%d", &noeud_emet);
    purge_stdinbuf();
    while((noeud_emet>99)||(noeud_emet<0))
    {
      printf("Noeud émetteur non valide, veuillez recommencer\n");
      noeud_emet=saisie_noeud(0);
    }
    return noeud_emet;
  }
  if(mode==1)
  {
    int noeud_dest;
    scanf("%d", &noeud_dest);
    purge_stdinbuf();
    while((noeud_dest>99)||(noeud_dest<0))
    {
      printf("Noeud destinataire non valide, veuillez recommencer\n");
      noeud_dest = saisie_noeud(1);
    }
    return noeud_dest;
  }
  return 0;
}

void affiche_chemin(int emet,int transit[]){
  int cpt=0;
  for(int i=0; i<10; i++){
    if(transit[i]!=-1) cpt++;
  }
printf("\nChemin emprunté:\n\n");
  printf("   [%d]--->",emet);
  for(int i=0;i<cpt;i++){
    if(i==cpt-1) printf("[%d]\n\n",transit[i]);
    else printf("(%d)--->",transit[i]);
  } 
}

void retrouve_chemin(NOEUD graphe, int nbsommets)
{ 
  printf("-----------------------------------------\n");
  printf("Les noeuds Tier 1 sont numérotés de 0 à 9\n");
  printf("Les noeuds Tier 2 sont numérotés de 10 à 29\n");
  printf("Les noeuds Tier 3 sont numérotés de 30 à 99\n");
  printf("-----------------------------------------\n");
  printf("Veuillez saisir un noeud émetteur \n");
  int noeud_emet = saisie_noeud(0); 
  printf("-----------------------------------------\n");
  printf("Veuillez saisir un noeud destinataire\n");
  int noeud_dest = saisie_noeud(1);
  printf("-----------------------------------------\n");
  if(noeud_dest==noeud_emet) 
    {
    printf("Les noeuds saisis sont identiques, veuillez saisir un noeud destinaire différent\n");
     saisie_noeud(1);
    }
  
  printf("Pour arriver au noeud %d l'information passe par les noeuds :\n", noeud_dest); 
  int transit[10];
  int n=0;
  int emet=noeud_emet;
  for(int i=0;i<10;i++) transit[i]=-1;
  while(graphe[noeud_emet].nom!=graphe[noeud_dest].nom)
  {
    printf("%d\n",graphe[noeud_emet].tableRoutage[1][noeud_dest]%100);
    transit[n]=graphe[noeud_emet].tableRoutage[1][noeud_dest]%100; n++;
    noeud_emet=(graphe[noeud_emet].tableRoutage[1][noeud_dest])%100;
  }
  printf("-----------------------------------------\n");
  affiche_chemin(emet,transit);
  printf("-----------------------------------------\n");
}

