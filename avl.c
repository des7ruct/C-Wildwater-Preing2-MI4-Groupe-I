#include <stdlib.h>
#include <string.h>
#include "avl.h"

/*/////////////////////////////////////////////////////////*/
/*             fonctions de base pour les AVL              */
/*/////////////////////////////////////////////////////////*/

int hauteur(AVL *n) {
    if (n != NULL) {
        return n->hauteur;
    }
    else {
        return 0;
    }
}

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    else {
        return b;
    }
}

char *dupliquer_chaine(char *a) {
    if (a == NULL) {
        return NULL;
    }

    int longueur = 0;
    while (a[longueur] != '\0') {
        longueur++;
    }

    char *copie = (char *)malloc(longueur + 1);
    if (copie == NULL) {
        return NULL;
    }

    for (int i = 0; i <= longueur; i++) {
        copie[i] = a[i];
    }

    return copie;
}

AVL *nouveau_noeud(char *cle, Usine *data) {
    AVL *n = malloc(sizeof(AVL));
    if (n == NULL) {
        return NULL;
    }

    n->cle = dupliquer_chaine(cle);
    n->data = data;
    n->hauteur = 1;
    n->gauche = NULL;
    n->droite = NULL;

    return n;
}

AVL *rotation_droite(AVL *y) {
    AVL *x = y->gauche;
    AVL *t = x->droite;

    x->droite = y;
    y->gauche = t;

    y->hauteur = max(hauteur(y->gauche), hauteur(y->droite)) + 1;
    x->hauteur = max(hauteur(x->gauche), hauteur(x->droite)) + 1;

    return x;
}

AVL *rotation_gauche(AVL *x) {
    AVL *y = x->droite;
    AVL *t = y->gauche;

    y->gauche = x;
    x->droite = t;

    x->hauteur = max(hauteur(x->gauche), hauteur(x->droite)) + 1;
    y->hauteur = max(hauteur(y->gauche), hauteur(y->droite)) + 1;

    return y;
}

int balance(AVL *n) {
    if (n == NULL) {
        return 0;
    }

    return hauteur(n->gauche) - hauteur(n->droite);
}

AVL *insert_AVL(AVL *noeud, char *cle, Usine *data) {
    if (noeud == NULL) {
        return nouveau_noeud(cle, data);
    }

    int comparaison = strcmp(cle, noeud->cle);

    if (comparaison < 0) {
        noeud->gauche = insert_AVL(noeud->gauche, cle, data);
    }
    else if (comparaison > 0) {
        noeud->droite = insert_AVL(noeud->droite, cle, data);
    }
    else {
        return noeud;
    }

    noeud->hauteur = 1 + max(hauteur(noeud->gauche), hauteur(noeud->droite));
    int equilibre = balance(noeud);

    if (equilibre > 1 && strcmp(cle, noeud->gauche->cle) < 0) {
        return rotation_droite(noeud);
    }

    if (equilibre < -1 && strcmp(cle, noeud->droite->cle) > 0) {
        return rotation_gauche(noeud);
    }

    if (equilibre > 1 && strcmp(cle, noeud->gauche->cle) > 0) {
        noeud->gauche = rotation_gauche(noeud->gauche);
        return rotation_droite(noeud);
    }

    if (equilibre < -1 && strcmp(cle, noeud->droite->cle) < 0) {
        noeud->droite = rotation_droite(noeud->droite);
        return rotation_gauche(noeud);
    }

    return noeud;
}

Usine *avl_recherche(AVL *noeud, char *cle) {
    if (noeud == NULL) {
        return NULL;
    }

    int comparaison = strcmp(cle, noeud->cle);

    if (comparaison == 0) {
        return noeud->data;
    }

    if (comparaison < 0) {
        return avl_recherche(noeud->gauche, cle);
    }
    else {
        return avl_recherche(noeud->droite, cle);
    }
}

void parcoursInfixeInverseAvl(AVL *n, FILE *sortie, int mode) {
    if (n == NULL) {
        return;
    }

    parcoursInfixeInverseAvl(n->droite, sortie, mode);

    if (mode == 0) {
        fprintf(sortie, "%s;%d\n", n->data->id, n->data->vol_max);
    }
    else if (mode == 1) {
        fprintf(sortie, "%s;%d\n", n->data->id, n->data->vol_src);
    }
    else {
        fprintf(sortie, "%s;%d\n", n->data->id, n->data->vol_real);
    }

    parcoursInfixeInverseAvl(n->gauche, sortie, mode);
}

void liberer_avl(AVL *n) {
    if (n == NULL) {
        return;
    }

    liberer_avl(n->gauche);
    liberer_avl(n->droite);

    free(n->data->id);
    free(n->data);
    free(n->cle);
    free(n);
}
