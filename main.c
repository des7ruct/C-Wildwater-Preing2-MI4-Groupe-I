#include <stdio.h>
#include <string.h>

#include "histogramme.h"
#include "leaks.h"

int main(int argc, char *argv[]) {

    /* ===================== MODE HISTOGRAMME ===================== */
    if (argc == 3 && strcmp(argv[1], "histo") == 0) {

        char *mode = argv[2];
        char *fichier_csv = "data.dat";

        if (strcmp(mode, "max") != 0 &&
            strcmp(mode, "src") != 0 &&
            strcmp(mode, "real") != 0) {

            printf("Mode histogramme invalide.\n");
            printf("Utilisation : ./projet_final histo <max|src|real>\n");
            return 1;
        }

        int res = traiter_histogramme(fichier_csv, mode);

        if (res != 0) {
            printf("Erreur lors du traitement de l'histogramme (code %d)\n", res);
            return res;
        }

        printf("Histogramme (%s) terminé avec succès.\n", mode);
        return 0;
    }

    /* ===================== MODE LEAKS ===================== */
    if (argc == 4 && strcmp(argv[1], "leaks") == 0) {

        char *fichier_csv = argv[2];
        char *id_usine = argv[3];

        float fuites = traiter_leaks(fichier_csv, id_usine);

        if (fuites < 0.0) {
        printf("%.2f\n", fuites);
        return 0;
}

printf("Fuites totales pour %s : %.2f\n", id_usine, fuites);
        return 0;
    }

    /* ===================== ERREUR D'USAGE ===================== */
    printf("Utilisation :\n");
    printf("  Histogrammes : ./projet_final histo <max|src|real>\n");
    printf("  Leaks        : ./projet_final leaks <csv> <id_usine>\n");

    return 1;
}
