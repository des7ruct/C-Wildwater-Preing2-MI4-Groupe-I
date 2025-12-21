#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
#include "outils.h"
#include "usine.h"
#include "histogramme.h"

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*   traiter_histogramme lit le fichier CSV ligne par ligne, construit un AVL d’usines, calcule pour chaque usine :     */
/*     - sa capacité maximale (vol_max)                                                                                 */
/*     - le volume capté par ses sources (vol_src)                                                                      */
/*     - le volume réellement traité (vol_real)                                                                         */
/*   écrit un fichier .dat correspondant au mode demandé et retourne le bon un code d’erreur                            */
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

int traiter_histogramme(char *fichier_csv, char *mode) {
    FILE *fichier = fopen(fichier_csv, "r");
    if (fichier == NULL) {
        return 1;
    }

    AVL *usines = NULL;
    char ligne[1024];

    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        char *colonnes[5] = {NULL};
        int indice = decouper_ligne(ligne, colonnes, 5);

        if (indice < 2) {
            continue;
        }

        for (int i = 0; i < indice; i++) {
            colonnes[i] = trim(colonnes[i]);
        }

        if (!valeur_absente(colonnes[1]) && valeur_absente(colonnes[2]) && !valeur_absente(colonnes[3]) && valeur_absente(colonnes[4])) {
            Usine *u = avl_recherche(usines, colonnes[1]);

            if (u == NULL) {
                u = malloc(sizeof(Usine));
                if (u == NULL) {
                    fclose(fichier);
                    return 2;
                }

                u->id = strdup(colonnes[1]);
                u->vol_max = 0;
                u->vol_src = 0;
                u->vol_real = 0;

                usines = insert_AVL(usines, u->id, u);
            }

            u->vol_max = chaine_vers_int(colonnes[3]);
        }
        else if (!valeur_absente(colonnes[1]) && !valeur_absente(colonnes[2]) && !valeur_absente(colonnes[3]) && !valeur_absente(colonnes[4])) {
            Usine *u = avl_recherche(usines, colonnes[2]);

            if (u == NULL) {
                u = malloc(sizeof(Usine));
                if (u == NULL) {
                    fclose(fichier);
                    return 3;
                }

                u->id = strdup(colonnes[2]);
                u->vol_max = 0;
                u->vol_src = 0;
                u->vol_real = 0;

                usines = insert_AVL(usines, u->id, u);
            }

            int volume = chaine_vers_int(colonnes[3]);
            float fuite = chaine_vers_float(colonnes[4]);

            u->vol_src += volume;
            u->vol_real += volume * (1.0 - fuite / 100.0);
        }
    }

    fclose(fichier);

    FILE *sortie = NULL;
    int mode_sortie = 0;

    if (strcmp(mode, "max") == 0) {
        sortie = fopen("vol_max.dat", "w");
        mode_sortie = 0;
    }
    else if (strcmp(mode, "src") == 0) {
        sortie = fopen("vol_src.dat", "w");
        mode_sortie = 1;
    }
    else if (strcmp(mode, "real") == 0) {
        sortie = fopen("vol_real.dat", "w");
        mode_sortie = 2;
    }
    else {
        return 4;
    }

    if (sortie == NULL) {
        return 5;
    }

    fprintf(sortie, "identifier;value\n");
    parcoursInfixeInverseAvl(usines, sortie, mode_sortie);
    fclose(sortie);

    liberer_avl(usines);

    return 0;
}
