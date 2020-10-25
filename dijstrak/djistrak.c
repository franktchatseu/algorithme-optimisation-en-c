#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define POIDS_MAX 50  //poids maximum
#define NB_SOM_MAX 10 //nombre de sommets maximum
#define TAILLE 5      /* nombre de sommets du graphe */
#define MAXINT 1000   /* un tres grand entier */
//liste chainée de couples (sommet, poids)
typedef struct maillon
{
    struct maillon *suiv;
    int nom;
    int poids;
} MAILLON, *LISTE;

//graph structure
typedef struct graphe
{
    int nbSommets;
    LISTE Adj[NB_SOM_MAX]; //liste d'adjacence
} GRAPHE;

int sommet_depart = 1;
int D[TAILLE];       /* Les distances trouvees a chaque instant */
int queue[TAILLE];   /* file pour les sommets selon D croissant */
int ordre[TAILLE];   /* Ordre de chaque sommet dans la file selon D croissant */
int explore[TAILLE]; /* Pour les sommets deja ajoutes */
int C[TAILLE];       /* pour le predecesseur de chaque sommet ajoute */

//insere (som_b,poids) en tete dans la liste d’adjacence Adj[som_a]
void insere(int som_a, int som_b, int poids, LISTE Adj[])
{
    LISTE prem = malloc(sizeof(LISTE));
    prem->nom = som_b;
    prem->poids = poids;
    prem->suiv = Adj[som_a];
    Adj[som_a] = prem;
}

//initialisation de la table d’adjacence : toutes les listes chainées sont vides
void initAdjGraphe(GRAPHE *G)
{
    int i;
    for (i = 0; i < G->nbSommets; i++)
    {
        G->Adj[i] = NULL;
    }
}

//pour charger un graphe a partir d’un fichier
void litGraphe(const char *adr, GRAPHE *G)
{
    FILE *f;
    int sa, sb, pds, nbArcs;
    f = fopen(adr, "r");

    if (f == NULL)
    {
        printf("File is missing!");
        exit(1);
    }

    fscanf(f, "%d", &(G->nbSommets));
    initAdjGraphe(G);
    fscanf(f, "%d", &nbArcs);
    //on recupere le sommet de depart
    fscanf(f, "%d", &sommet_depart);
    while (nbArcs)
    {
        fscanf(f, "%d %d %d", &sa, &sb, &pds);
        insere(sa, sb, pds, G->Adj);
        nbArcs--;
    }
    fclose(f);
}

// affichage d’un graphe : le nombre de sommets, puischaque arc pondéré : (sommet_1, sommet_2, poids)
void afficheGraphe(GRAPHE G)
{
    int j;
    printf("Nb sommets: %d\n", G.nbSommets);
    printf("sommets de depart: %d\n", sommet_depart);

    for (j = 0; j < G.nbSommets; j++)
    {
        MAILLON *tmp = G.Adj[j];
        while (tmp != NULL)
        {
            printf(" (%d, %d, %d)\n", j, tmp->nom, tmp->poids);
            tmp = tmp->suiv;
        }
    }
}

// affiche le plus court chemin de s à t et renvoie 0 s'il n'existe pas
int affiche_plus_court_chemin(int s, int t, int pred[])
{
    return 0;
}

/* Initialisation des diverses variables */
void initialisation(void)
{
    int i;
    for (i = 0; i < TAILLE; i++)
    {
        D[i] = MAXINT;
        queue[i] = i;
        ordre[i] = i;
        explore[i] = 0;
    }
    D[sommet_depart] = 0;
    C[sommet_depart] = -1; /* On commence par 0 */
}

/* Operation sur les files d'attente */
void echanger(int i, int j) /* echanger deux sommets dans la queue */
{
    int a;
    ordre[queue[i]] = j;
    ordre[queue[j]] = i;
    a = queue[i];
    queue[i] = queue[j];
    queue[j] = a;
}

/* retablit la condition de file de maniere descendante sur les longueur positions de la file */
void echange_descendant(int longueur, int i)
{
    int fils = 2 * i + 1;
    if (fils >= longueur)
        return;
    if ((fils < longueur - 1) && (D[queue[fils]] > D[queue[fils + 1]]))
        fils = fils + 1;
    if (D[queue[i]] > D[queue[fils]])
    {
        echanger(i, fils);
        echange_descendant(longueur, fils);
    }
}

/* retablit la condition de file de maniere ascendante sur les longueur positions de la file */
void echange_montant(int longueur, int i)
{
    int pere = (i - 1) / 2;
    if (D[queue[i]] < D[queue[pere]])
    {
        echanger(pere, i);
        echange_montant(longueur, pere);
    }
}

/* retourne le minimum dans la file et retablit l'ordre */
int minimum(int longueur)
{
    int a = queue[sommet_depart];
    explore[a] = 1;
    if (longueur > 1)
    {
        echanger(0, longueur - 1);
        echange_descendant(longueur - 1, 0);
    }
    return a;
}
/* algorithme de Dijkstra. Premiere etape, on ajoute un sommet, puis on actualise */
void dijkstra_iteration(int longueur, GRAPHE graphe) /* nombre de sommets non encore inclus */
{
    MAILLON *c;
    int a = minimum(longueur); /* extraction du minimum */
    int j;
    //on parcours tout les sommets adjacents
    for (c = graphe.Adj[a]; c != NULL; c = c->suiv)
    {
        j = c->nom;
        if ((explore[j] == 0) && (D[a] + c->poids < D[j]))
        {
            D[j] = D[a] + c->poids;
            C[j] = a;
            echange_montant(longueur - 1, ordre[j]); /* si actualisation */
        }
    }
}
void affiche_resultat()
{
    printf("<------ Resultat ------->\n");
    printf("partant du sommet %d on a:\n",sommet_depart);
    printf("\n");
    int i, total_dist;
    total_dist = 0;
    for (i = 0; i < TAILLE; i++)
    {
        total_dist += D[i];
        printf("source %d, dist: %d, dest %d\n", C[i], D[i], i);
    }
    printf("\n");
    printf("la distance total pour le plus court chemin est: %d", total_dist);
}

void dijkstra44(GRAPHE graphe)
{
    int i;
    initialisation();
    for (i = TAILLE; i > 0; i--)
        dijkstra_iteration(i, graphe);
}
int main(void)
{
    GRAPHE G;
    litGraphe("./file-distrack.txt", &G);
    afficheGraphe(G);
    printf("\n");
    dijkstra44(G);
    affiche_resultat();
    return 0;
}