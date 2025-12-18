/*/////////////////////////////////////////*/
/*   Nœud de l’arbre normal (réseau aval)  */
/*/////////////////////////////////////////*/

/* arbre.h */

#ifndef ARBRE_H
#define ARBRE_H

typedef struct NoeudArbre NoeudArbre;

/* Liste chaînée des enfants */
typedef struct Enfant {
    NoeudArbre *noeud;
    struct Enfant *suivant;
} Enfant;

/* Nœud du réseau de distribution */
struct NoeudArbre {
    double volume;              // Volume d'eau entrant
    double pourcentage_fuite;   // % de fuite du tronçon parent → ce nœud
    Enfant *enfants;            // Liste chaînée des enfants
};

NoeudArbre *creer_noeud_arbre(double volume, double fuite);
void ajouter_enfant(NoeudArbre *parent, NoeudArbre *enfant);

#endif

/*/////////////////////////////////////////*/
/*    Implémentation de l’arbre normal     */
/*/////////////////////////////////////////*/

/* arbre.c */

#include <stdlib.h>
#include "arbre.h"

NoeudArbre *creer_noeud_arbre(double volume, double fuite) {
    NoeudArbre *n = malloc(sizeof(NoeudArbre));
    if (!n) return NULL;

    n->volume = volume;
    n->pourcentage_fuite = fuite;
    n->enfants = NULL;
    return n;
}

void ajouter_enfant(NoeudArbre *parent, NoeudArbre *enfant) {
    Enfant *e = malloc(sizeof(Enfant));
    e->noeud = enfant;
    e->suivant = parent->enfants;
    parent->enfants = e;
}

/*/////////////////////////////////////////*/
/*          Nœud de l’AVL (index)          */
/*/////////////////////////////////////////*/

/* avl.h */

#ifndef AVL_H
#define AVL_H

#include "arbre.h"

typedef struct NoeudAVL {
    char *identifiant;          // Identifiant acteur aval
    NoeudArbre *noeud_arbre;    // Pointeur vers l’arbre normal
    int hauteur;
    struct NoeudAVL *gauche;
    struct NoeudAVL *droite;
} NoeudAVL;

NoeudAVL *avl_inserer(NoeudAVL *racine, const char *id, NoeudArbre *noeud);
NoeudArbre *avl_rechercher(NoeudAVL *racine, const char *id);

#endif

/*/////////////////////////////////////////*/
/*           Création d’un nœud            */
/*/////////////////////////////////////////*/ 

/*Le volume est :

égal à la valeur du CSV si c’est une ligne source → usine

égal à 0 sinon
*/

NoeudArbre *nouveau_noeud;

if (ligne_source_usine) {
    nouveau_noeud = creer_noeud_arbre(volume, fuite);
} else {
    nouveau_noeud = creer_noeud_arbre(0.0, fuite);
}

/*/////////////////////////////////////////*/
/*         Traitement d’un tronçon         */
/*/////////////////////////////////////////*/

void traiter_troncon(NoeudAVL **avl, NoeudArbre *racine_fictive, const char *id_amont, const char *id_aval, double volume, double fuite)
{
    /* Recherche du parent dans l'AVL */
    NoeudArbre *noeud_parent = avl_rechercher(*avl, id_amont);

    /* Création du nœud enfant */
    NoeudArbre *noeud_enfant = creer_noeud_arbre(volume, fuite);

    /* Placement dans l'arbre normal */
    if (noeud_parent != NULL) {
        ajouter_enfant(noeud_parent, noeud_enfant);
    } else {
        /* Parent inconnu → accroché à la racine fictive */
        ajouter_enfant(racine_fictive, noeud_enfant);
    }

    /* Insertion dans l'AVL (clé = acteur aval) */
    *avl = avl_inserer(*avl, id_aval, noeud_enfant);
}

/*/////////////////////////////////////////*/
/*      Détection ligne source → usine     */
/*/////////////////////////////////////////*/

int est_ligne_source_usine(char **colonnes) {
    return strstr(colonnes[1], "Spring") != NULL;
}

/*/////////////////////////////////////////*/
/*        Traitement des lignes CSV        */
/*/////////////////////////////////////////*/

if (est_ligne_source_usine(colonnes)) {
    /* SOURCE → USINE */
    if (strcmp(colonnes[2], identifiant_usine) == 0) {
        traiter_troncon(
            &avl,
            racine,
            colonnes[1],           // acteur amont (source)
            colonnes[2],           // acteur aval (usine)
            atof(colonnes[3]),
            atof(colonnes[4])
        );
    }
} else {
    /* AUTRES TRONÇONS */
    if (strcmp(colonnes[0], identifiant_usine) == 0) {
        traiter_troncon(
            &avl,
            racine,
            colonnes[1],           // acteur amont
            colonnes[2],           // acteur aval
            0.0,
            atof(colonnes[4])
        );
    }
}


/*/////////////////////////////////////////*/
/*        Racine fictive obligatoire       */
/*/////////////////////////////////////////*/

NoeudArbre *racine = creer_noeud_arbre(0.0, 0.0);

/*//////////////////////////////////////////////////*/
/*     Parcours récursif pour calculer les pertes   */
/*//////////////////////////////////////////////////*/

double calculer_fuites(NoeudArbre *noeud, double volume_entrant);

////////////////


