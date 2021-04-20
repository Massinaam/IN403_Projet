
// Structure de donnée stockant un noeud
struct Noeud
{
	int nom;//nom du noeud considéré
	int nb_voisin; // nombre de voisins, représente aussi la position du nouveau noeud à insérer
	struct Noeud* suivant;// pointe le voisin suivant dans la liste chaînée le cas échéant
	int* poids;// ième poids de l'arrête entre le noeud et le ième voisin du noeud
  int tableRoutage[2][100]; // table de routage du noeud
};typedef struct Noeud* NOEUD;

//Création d'un graphe non orienté vide à 100 sommets
NOEUD creerGraphe(int nbsommets);

int different(struct Noeud n1,NOEUD suivant, int nb_voisins);

//Implémentation du graphe et création des arrêtes
NOEUD implementationGraphe(NOEUD graphe,int nbsommets);

int voisin_tiers2(struct Noeud n);

// écrase graphe courant
void libererGraphe(NOEUD graphe, int nbsommets);

// affiche les noeuds du graphe courant, et ses liens éventuels avec ses voisins
void affiche_noeuds(NOEUD n, int nbsommets);

void explorer(struct Noeud s, int T[100], int i);

int marquage(NOEUD G, int nbsommets);

int dans_min(int min[100], int j, int min_tab);

int min_poids(int d[100], int min[100], int min_tab, int nbsommets);

int verif_arete(struct Noeud n, int nom);

NOEUD liste_destinataire(NOEUD graphe, int nbsommets);

NOEUD calcul_distance(NOEUD graphe, int sommets, struct Noeud n);

NOEUD table_routage_1(NOEUD graphe, int nbsommets);

NOEUD tableRoutage2(NOEUD graphe, struct Noeud n, int nbsommets, int d[100], int pred[100]);

NOEUD tableRoutage(NOEUD graphe, int nbsommets);