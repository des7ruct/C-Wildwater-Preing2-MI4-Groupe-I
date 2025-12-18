#include <stdio.h>
#include "histogramme.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <mode>\n", argv[0]);
        printf("Mode: max, src, real\n");
        return 1;
    }

    char *fichier_csv = "data.dat";
    char *mode = argv[1];

    int resultat = traiter_histogramme(fichier_csv, mode);

    if (resultat != 0) {
        printf("Erreur lors du traitement du fichier (code %d)\n", resultat);
        return resultat;
    }

    printf("Traitement terminé avec succès.\n");
    return 0;
}
