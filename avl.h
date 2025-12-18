#ifndef AVL_H
#define AVL_H

#include <stdio.h>
#include "usine.h"

typedef struct AVL {
    char *cle;
    Usine *data;
    int hauteur;
    struct AVL *gauche;
    struct AVL *droite;
} AVL;

AVL *insert_AVL(AVL *noeud, char *cle, Usine *data);
Usine *avl_recherche(AVL *noeud, char *cle);
void liberer_avl(AVL *n);
void parcoursInfixeInverseAvl(AVL *n, FILE *sortie, int mode);

#endif
