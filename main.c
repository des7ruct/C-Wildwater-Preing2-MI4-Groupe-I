#include <stdio.h>
#include <string.h>
#include <time.h>

#include "histogramme.h"
#include "leaks.h"
#include "outils.h"

int main(int argc, char *argv[]) {
    clock_t debut, fin ;
    double temps_cpu ;
    debut = clock() ;

    /* ===================== MODE HISTOGRAMME ===================== */
    if (argc == 4 && strcmp(argv[1], "histo") == 0) {

        char *fichier_csv = argv[2];
        char *mode = argv[3];

        if (strcmp(mode, "max") != 0 &&
            strcmp(mode, "src") != 0 &&
            strcmp(mode, "real") != 0) {

            printf("Mode histogramme invalide.\n");
            printf("Utilisation : ./projet_final <csv> histo <max|src|real>\n");

            fin = clock() ;
            temps_cpu = ((double)(fin - debut)) * 1000.0 / CLOCKS_PER_SEC ;
            printf("Temps écoulé : %.2f ms\n", temps_cpu);
            return 1;
        }

    

        int res = traiter_histogramme(fichier_csv, mode);

        if (res != 0) {
            printf("Erreur lors du traitement de l'histogramme (code %d)\n", res);

            fin = clock() ;
            temps_cpu = ((double)(fin - debut)) * 1000.0 / CLOCKS_PER_SEC ;
            printf("Temps écoulé : %.2f ms\n", temps_cpu);
            return res;
        }

        printf("Histogramme (%s) terminé avec succès.\n", mode);
        fin = clock() ;
        temps_cpu = ((double)(fin - debut)) * 1000.0 / CLOCKS_PER_SEC ;
        printf("Temps écoulé : %.2f ms\n", temps_cpu);
        return 0;
    }

    /* ===================== MODE LEAKS ===================== */
    if (argc == 4 && strcmp(argv[1], "leaks") == 0) {

        char *fichier_csv = argv[2];
        char *id_usine = argv[3];


        float fuites = traiter_leaks(fichier_csv, id_usine);

        
        if (fuites < 0.0) {
            printf("%.2f\n", fuites);
            fin = clock() ;
            temps_cpu = ((double)(fin - debut)) * 1000.0 / CLOCKS_PER_SEC ;
            printf("Temps écoulé : %.2f ms\n", temps_cpu);
            return 0;
        }

        printf("Fuites totales pour %s : %.2f\n", id_usine, fuites);
        fin = clock() ;
        temps_cpu = ((double)(fin - debut)) * 1000.0 / CLOCKS_PER_SEC ;
        printf("Temps écoulé : %.2f ms\n", temps_cpu);
        return 0;
    }

    /* ===================== ERREUR D'USAGE ===================== */
    printf("Utilisation :\n");
    printf("  Histogrammes : ./projet_final <csv> histo <max|src|real>\n");
    printf("  Leaks        : ./projet_final <csv> leaks <id_usine>\n");
    fin = clock() ;
    temps_cpu = ((double)(fin - debut)) * 1000.0 / CLOCKS_PER_SEC ;
    printf("Temps écoulé : %.2f ms\n", temps_cpu);
    return 1;
}
