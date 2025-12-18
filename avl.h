#ifndef AVL_H
#define AVL_H

#include "types.h"
#include <stdio.h>

AVL *insert_AVL(AVL *noeud, char *cle, Usine *data);
Usine *avl_recherche(AVL *noeud, char *cle);
void parcoursInfixeInverseAvl(AVL *n, FILE *sortie, int mode);
void liberer_avl(AVL *n);

#endif
