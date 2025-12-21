#ifndef LEAKS_H
#define LEAKS_H

#include "avl.h"
#include "usine.h"

/* ======================= TRONCON ======================= */

typedef struct Troncon Troncon;

struct Troncon {
    char *id;
    float volume;
    float fuite;
    Troncon *enfants;
    Troncon *suivant;
};

/* ======================= AVL TRONCON ======================= */

typedef struct AVL_Troncon AVL_Troncon;

struct AVL_Troncon {
    char *cle;
    Troncon *data;
    int hauteur;
    AVL_Troncon *gauche;
    AVL_Troncon *droite;
};

/* ======================= TRONCON ======================= */

Troncon *nouveau_troncon(char *id, float volume, float fuite);
void ajouter_enfant(Troncon *parent, Troncon *enfant);
float calculer_fuites(Troncon *n);
void liberer_troncon(Troncon *n);

/* ======================= AVL TRONCON ======================= */

AVL_Troncon *insert_AVL_troncon(AVL_Troncon *n, char *cle, Troncon *data);
Troncon *avl_troncon_recherche(AVL_Troncon *n, char *cle);
void liberer_avl_troncon(AVL_Troncon *n);

/* ======================= LEAKS ======================= */

float traiter_leaks(char *fichier_csv, char *id_usine);

#endif
