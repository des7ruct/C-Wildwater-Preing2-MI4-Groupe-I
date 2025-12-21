#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "leaks.h"
#include "outils.h"

/*////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*    Pour une usine id_usine le programme charge les usines et calcule leurs volumes,                    */
/*      il vérifie que l’usine existe, il construit un graphe aval (arbre à enfants multiples),           */
/*      il utilise un AVL pour retrouver chaque tronçon rapidement, répartit les volumes équitablement,   */
/*      calcule récursivement les fuites, enregistre le résultat et libère toute la mémoire               */
/*////////////////////////////////////////////////////////////////////////////////////////////////////////*/

AVL *charger_usines(char *fichier_csv) {
    FILE *fichier = fopen(fichier_csv, "r");
    if (fichier == NULL) {
        return NULL;
    }

    AVL *usines = NULL;
    char ligne[1024];

    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        char *cols[5] = { NULL };
        int nb = decouper_ligne(ligne, cols, 5);

        for (int i = 0; i < nb; i++) {
            cols[i] = trim(cols[i]);
        }

      
        if (nb >= 4 &&
            valeur_absente(cols[1]) == 0 &&
            valeur_absente(cols[2]) == 1 &&
            valeur_absente(cols[3]) == 0) {

            Usine *u = avl_recherche(usines, cols[1]);

            if (u == NULL) {
                u = malloc(sizeof(Usine));
                if (u == NULL) {
                    fclose(fichier);
                    return NULL;
                }

                u->id = dupliquer_chaine(cols[1]);
                u->vol_max = 0;
                u->vol_src = 0;
                u->vol_real = 0;

                usines = insert_AVL(usines, u->id, u);
            }

            u->vol_max = chaine_vers_int(cols[3]);
        }

       
        else if (nb >= 5 &&
                 valeur_absente(cols[1]) == 0 &&
                 valeur_absente(cols[2]) == 0 &&
                 valeur_absente(cols[3]) == 0 &&
                 valeur_absente(cols[4]) == 0) {

            Usine *u = avl_recherche(usines, cols[2]);

            if (u == NULL) {
                u = malloc(sizeof(Usine));
                if (u == NULL) {
                    fclose(fichier);
                    return NULL;
                }

                u->id = dupliquer_chaine(cols[2]);
                u->vol_max = 0;
                u->vol_src = 0;
                u->vol_real = 0;

                usines = insert_AVL(usines, u->id, u);
            }

            int volume = chaine_vers_int(cols[3]);
            float fuite = chaine_vers_float(cols[4]);

            u->vol_src += volume;
            u->vol_real += volume * (1.0 - fuite / 100.0);
        }
    }

    fclose(fichier);
    return usines;
}



Troncon *nouveau_troncon(char *id, float volume, float fuite) {
    Troncon *t = malloc(sizeof(Troncon));
    if (t == NULL) {
        return NULL;
    }

    t->id = dupliquer_chaine(id);
    t->volume = volume;
    t->fuite = fuite;
    t->enfants = NULL;
    t->suivant = NULL;

    return t;
}

void ajouter_enfant(Troncon *parent, Troncon *enfant) {
    if (parent->enfants == NULL) {
        parent->enfants = enfant;
    } else {
        Troncon *p = parent->enfants;
        while (p->suivant != NULL) {
            p = p->suivant;
        }
        p->suivant = enfant;
    }
}

float calculer_fuites(Troncon *n) {
    if (n == NULL) {
        return 0.0;
    }

    int nb_enfants = 0;
    float fuites_enfants = 0.0;

    Troncon *c = n->enfants;
    while (c != NULL) {
        nb_enfants++;
        c = c->suivant;
    }

    if (nb_enfants == 0) {
        return n->volume * (n->fuite / 100.0);
    }

    c = n->enfants;
    while (c != NULL) {
        c->volume = n->volume / nb_enfants;
        fuites_enfants += calculer_fuites(c);
        c = c->suivant;
    }

    return n->volume * (n->fuite / 100.0) + fuites_enfants;
}

void liberer_troncon(Troncon *n) {
    if (n == NULL) {
        return;
    }

    Troncon *c = n->enfants;
    while (c != NULL) {
        Troncon *suiv = c->suivant;
        liberer_troncon(c);
        c = suiv;
    }

    free(n->id);
    free(n);
}



int hauteur_troncon(AVL_Troncon *n) {
    if (n == NULL) {
        return 0;
    }
    return n->hauteur;
}

int max_troncon(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

AVL_Troncon *rotation_droite_troncon(AVL_Troncon *y) {
    AVL_Troncon *x = y->gauche;
    AVL_Troncon *t = x->droite;

    x->droite = y;
    y->gauche = t;

    y->hauteur = max_troncon(hauteur_troncon(y->gauche), hauteur_troncon(y->droite)) + 1;
    x->hauteur = max_troncon(hauteur_troncon(x->gauche), hauteur_troncon(x->droite)) + 1;

    return x;
}

AVL_Troncon *rotation_gauche_troncon(AVL_Troncon *x) {
    AVL_Troncon *y = x->droite;
    AVL_Troncon *t = y->gauche;

    y->gauche = x;
    x->droite = t;

    x->hauteur = max_troncon(hauteur_troncon(x->gauche), hauteur_troncon(x->droite)) + 1;
    y->hauteur = max_troncon(hauteur_troncon(y->gauche), hauteur_troncon(y->droite)) + 1;

    return y;
}

int equilibre_troncon(AVL_Troncon *n) {
    if (n == NULL) {
        return 0;
    }
    return hauteur_troncon(n->gauche) - hauteur_troncon(n->droite);
}

AVL_Troncon *insert_AVL_troncon(AVL_Troncon *n, char *cle, Troncon *data) {
    if (n == NULL) {
        AVL_Troncon *nv = malloc(sizeof(AVL_Troncon));
        if (nv == NULL) {
            return NULL;
        }

        nv->cle = dupliquer_chaine(cle);
        nv->data = data;
        nv->hauteur = 1;
        nv->gauche = NULL;
        nv->droite = NULL;

        return nv;
    }

    int cmp = strcmp(cle, n->cle);

    if (cmp < 0) {
        n->gauche = insert_AVL_troncon(n->gauche, cle, data);
    } else if (cmp > 0) {
        n->droite = insert_AVL_troncon(n->droite, cle, data);
    } else {
        return n;
    }

    n->hauteur = 1 + max_troncon(
        hauteur_troncon(n->gauche),
        hauteur_troncon(n->droite)
    );

    int eq = equilibre_troncon(n);

    if (eq > 1 && strcmp(cle, n->gauche->cle) < 0) {
        return rotation_droite_troncon(n);
    }

    if (eq < -1 && strcmp(cle, n->droite->cle) > 0) {
        return rotation_gauche_troncon(n);
    }

    if (eq > 1 && strcmp(cle, n->gauche->cle) > 0) {
        n->gauche = rotation_gauche_troncon(n->gauche);
        return rotation_droite_troncon(n);
    }

    if (eq < -1 && strcmp(cle, n->droite->cle) < 0) {
        n->droite = rotation_droite_troncon(n->droite);
        return rotation_gauche_troncon(n);
    }

    return n;
}

Troncon *avl_troncon_recherche(AVL_Troncon *n, char *cle) {
    if (n == NULL) {
        return NULL;
    }

    int cmp = strcmp(cle, n->cle);

    if (cmp == 0) {
        return n->data;
    }

    if (cmp < 0) {
        return avl_troncon_recherche(n->gauche, cle);
    }

    return avl_troncon_recherche(n->droite, cle);
}

void liberer_avl_troncon(AVL_Troncon *n) {
    if (n == NULL) {
        return;
    }

    liberer_avl_troncon(n->gauche);
    liberer_avl_troncon(n->droite);

    free(n->cle);
    free(n);
}



float traiter_leaks(char *fichier_csv, char *id_usine) {
    AVL *avl_usines = charger_usines(fichier_csv);
    if (avl_usines == NULL) {
        return -1.0;
    }

    Usine *u = avl_recherche(avl_usines, id_usine);
    if (u == NULL) {
        liberer_avl(avl_usines);
        return -2.0;
    }

    FILE *f = fopen(fichier_csv, "r");
    if (f == NULL) {
        liberer_avl(avl_usines);
        return -3.0;
    }

    Troncon *racine = nouveau_troncon(u->id, u->vol_real, 0.0);
    AVL_Troncon *avl_troncons = NULL;

    avl_troncons = insert_AVL_troncon(avl_troncons, racine->id, racine);

    char ligne[1024];

    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        char *cols[5] = { NULL };
        int nb = decouper_ligne(ligne, cols, 5);

        for (int i = 0; i < nb; i++) {
            cols[i] = trim(cols[i]);
        }

        if (nb < 5 || valeur_absente(cols[1]) || valeur_absente(cols[2])) {
            continue;
        }

        float fuite = chaine_vers_float(cols[4]);

        Troncon *parent = avl_troncon_recherche(avl_troncons, cols[1]);
        if (parent == NULL) {
            continue;
        }

        Troncon *enfant = avl_troncon_recherche(avl_troncons, cols[2]);
        if (enfant == NULL) {
            enfant = nouveau_troncon(cols[2], 0.0, fuite);
            avl_troncons = insert_AVL_troncon(avl_troncons, enfant->id, enfant);
        }

        ajouter_enfant(parent, enfant);
    }

    fclose(f);

    /* ================= CALCUL DES FUITES ================= */

    float fuites = calculer_fuites(racine);

    /* ================= SAUVEGARDE AUTOMATIQUE ================= */

    FILE *out = fopen("leaks.dat", "a");
    if (out != NULL) {
        fprintf(out, "%s;%.2f\n", id_usine, fuites);
        fclose(out);
    }

    /* ================= LIBÉRATIONS ================= */

    liberer_troncon(racine);
    liberer_avl_troncon(avl_troncons);
    liberer_avl(avl_usines);

    return fuites;
}


