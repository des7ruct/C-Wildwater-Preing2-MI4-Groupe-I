#include <stdlib.h>
#include <string.h>
#include "avl.h"

static int hauteur(AVL *n) {
    return n ? n->hauteur : 0;
}

static int max(int a, int b) {
    return a > b ? a : b;
}

static char *dupliquer_chaine(char *a) {
    if (!a) return NULL;
    int len = 0;
    while (a[len]) len++;
    char *copie = malloc(len + 1);
    if (!copie) return NULL;
    for (int i = 0; i <= len; i++) copie[i] = a[i];
    return copie;
}

static AVL *nouveau_noeud(char *cle, Usine *data) {
    AVL *n = malloc(sizeof(AVL));
    if (!n) return NULL;
    n->cle = dupliquer_chaine(cle);
    n->data = data;
    n->hauteur = 1;
    n->gauche = NULL;
    n->droite = NULL;
    return n;
}

static AVL *rotation_droite(AVL *y) {
    AVL *x = y->gauche;
    AVL *t = x->droite;
    x->droite = y;
    y->gauche = t;
    y->hauteur = max(hauteur(y->gauche), hauteur(y->droite)) + 1;
    x->hauteur = max(hauteur(x->gauche), hauteur(x->droite)) + 1;
    return x;
}

static AVL *rotation_gauche(AVL *x) {
    AVL *y = x->droite;
    AVL *t = y->gauche;
    y->gauche = x;
    x->droite = t;
    x->hauteur = max(hauteur(x->gauche), hauteur(x->droite)) + 1;
    y->hauteur = max(hauteur(y->gauche), hauteur(y->droite)) + 1;
    return y;
}

static int balance(AVL *n) {
    if (!n) return 0;
    return hauteur(n->gauche) - hauteur(n->droite);
}

AVL *insert_AVL(AVL *noeud, char *cle, Usine *data) {
    if (!noeud) return nouveau_noeud(cle, data);

    int cmp = strcmp(cle, noeud->cle);
    if (cmp < 0) noeud->gauche = insert_AVL(noeud->gauche, cle, data);
    else if (cmp > 0) noeud->droite = insert_AVL(noeud->droite, cle, data);
    else return noeud;

    noeud->hauteur = 1 + max(hauteur(noeud->gauche), hauteur(noeud->droite));
    int eq = balance(noeud);

    if (eq > 1 && strcmp(cle, noeud->gauche->cle) < 0) return rotation_droite(noeud);
    if (eq < -1 && strcmp(cle, noeud->droite->cle) > 0) return rotation_gauche(noeud);
    if (eq > 1 && strcmp(cle, noeud->gauche->cle) > 0) {
        noeud->gauche = rotation_gauche(noeud->gauche);
        return rotation_droite(noeud);
    }
    if (eq < -1 && strcmp(cle, noeud->droite->cle) < 0) {
        noeud->droite = rotation_droite(noeud->droite);
        return rotation_gauche(noeud);
    }

    return noeud;
}

Usine *avl_recherche(AVL *noeud, char *cle) {
    if (!noeud) return NULL;
    int cmp = strcmp(cle, noeud->cle);
    if (cmp == 0) return noeud->data;
    if (cmp < 0) return avl_recherche(noeud->gauche, cle);
    return avl_recherche(noeud->droite, cle);
}

void parcoursInfixeInverseAvl(AVL *n, FILE *sortie, int mode) {
    if (!n) return;
    parcoursInfixeInverseAvl(n->droite, sortie, mode);
    if (mode == 0) fprintf(sortie, "%s;%d\n", n->data->id, n->data->vol_max);
    else if (mode == 1) fprintf(sortie, "%s;%d\n", n->data->id, n->data->vol_src);
    else fprintf(sortie, "%s;%d\n", n->data->id, n->data->vol_real);
    parcoursInfixeInverseAvl(n->gauche, sortie, mode);
}

void liberer_avl(AVL *n) {
    if (!n) return;
    liberer_avl(n->gauche);
    liberer_avl(n->droite);
    free(n->data->id);
    free(n->data);
    free(n->cle);
    free(n);
}
