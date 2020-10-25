#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NB_ARC 50     //poids maximum
#define NB_SOM_MAX 50 //nombre de sommets maximum

//liste chainée de couples (sommet source, sommet destination, poids)
typedef struct arete
{
    int som_source;
    int som_dest;
    int poids;
} ARETE;

typedef struct graph
{
    int nbSommets;
    int nbAretes;
    ARETE Liste[NB_SOM_MAX];
} Graph;

//pour charger un graphe a partir d’un fichier
void litGraphe(const char *adr, Graph *G)
{
    FILE *f;
    int sa, sb, pds;
    f = fopen(adr, "r");
    if (f == NULL)
    {
        printf("File is missing!");
        exit(1);
    }

    fscanf(f, "%d", &(G->nbSommets));
    // initAdjGraphe(G);
    fscanf(f, "%d", &(G->nbAretes));
   // printf("le nombre arc est %d \n", G->nbAretes);
    int i = 0;
    for (i = 0; i < G->nbAretes; i++)
    {
        fscanf(f, "%d %d %d", &sa, &sb, &pds);
        G->Liste[i].som_source = sa;
        G->Liste[i].som_dest = sb;
        G->Liste[i].poids = pds;
    }

    fclose(f);
}
void afficherGraph(Graph G)
{
    int i = 0;
    for (i = 0; i < G.nbAretes; i++)
    {
        printf("[%d]<--(%d)-->[%d]\n", G.Liste[i].som_source, G.Liste[i].poids, G.Liste[i].som_dest);
    }
}
//trie des aretes dans ordre croissant des poids
void triListArete(Graph *G)
{
    int i, j;
    ARETE valeurtemporaire;

    for (i = 0; i < G->nbAretes; i++)
    {
        for (j = i; j < G->nbAretes; j++)
        {
            if (G->Liste[j].poids < G->Liste[i].poids) /* si on inverse le signe d'inégalité
                                          on aura le trie décroissant */
            {
                valeurtemporaire = G->Liste[i];
                G->Liste[i] = G->Liste[j];
                G->Liste[j] = valeurtemporaire;
            }
        }
    }
}

int findParent(int sommet, int *tab)
{
    if(sommet == tab[sommet])
        return sommet;
    return findParent(tab[sommet],tab);
}

void kruskal(Graph G)
{
    //on trie les aretes du graph dans ordre poids croissant
    triListArete(&G);
    Graph output;
    output.Liste[G.nbSommets-1];
    output.nbAretes = G.nbSommets - 1;
    output.nbSommets = G.nbSommets;
    int* parent;
    parent = malloc(sizeof(int)*output.nbSommets);
    //initialisation des parents
    int i = 0;
    for (i = 0; i < output.nbSommets; i++)
    {
        parent[i] = i;
    }
    i = 0;
    int count = 0;
    while (count != output.nbSommets - 1)
    {
        //on choisit arete qui a le plus petit cout
        ARETE arete = G.Liste[i];
        //on verifie si arbre formera un cycle avec cet arete
        int sourcePArent = findParent(arete.som_source, parent);
        int sourceDest = findParent(arete.som_dest, parent);
        if (sourcePArent != sourceDest)
        {
            output.Liste[count] = arete;
            count++;
            parent[sourcePArent] = sourceDest;
        }
        i++;
    }
     afficherGraph(output);
     free(parent);
}
int main(void)
{
    int i;
    Graph G;
    litGraphe("graphe1.txt", &G);
    kruskal(G);
    
    return 0;
}
