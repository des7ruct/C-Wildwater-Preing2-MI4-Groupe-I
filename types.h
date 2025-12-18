#ifndef TYPES_H
#define TYPES_H

typedef struct {
    char *id;
    int vol_max;
    int vol_src;
    int vol_real;
} Usine;

typedef struct AVL {
    char *cle;
    Usine *data;
    int hauteur;
    struct AVL *gauche;
    struct AVL *droite;
} AVL;

#endif
