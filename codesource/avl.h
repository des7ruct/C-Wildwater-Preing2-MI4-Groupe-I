#ifndef AVL_H
#define AVL_H

#include <stdio.h>
#include "usine.h"

// déclaration de la structure AVL ainsi que de différentes fonctions

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
int hauteur(AVL *n);
int max(int a, int b);
char *dupliquer_chaine(char *a);
AVL *nouveau_noeud(char *cle, Usine *data);
AVL *rotation_droite(AVL *y);
AVL *rotation_gauche(AVL *x);
int balance(AVL *n);

#endif
