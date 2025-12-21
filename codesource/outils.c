#include <string.h>
#include "outils.h"

/*/////////////////////////////////////////////////////////////////*/
/*    Ce fichier est composé de fonctions permettant de détecter   */
/*    les valeurs absentes (-), de nettoyer les chaînes lues       */
/*    dans le CSV et de découper une ligne CSV                     */
/*/////////////////////////////////////////////////////////////////*/

int valeur_absente(char *a) {
    if (a == NULL) {
        return 1;
    }

    if (*a == '\0') {
        return 1;
    }

    if (*a == '\n') {
        return 1;
    }

    if (strcmp(a, "-") == 0) {
        return 1;
    }

    return 0;
}

char *trim(char *s) {
    int debut = 0;
    int fin = strlen(s) - 1;

    while (s[debut] == ' ' || s[debut] == '\t') {
        debut++;
    }

    while (fin >= debut && (s[fin] == ' ' || s[fin] == '\t' || s[fin] == '\n' || s[fin] == '\r')) {
        fin--;
    }

    s[fin + 1] = '\0';
    return s + debut;
}

int decouper_ligne(char *ligne, char *colonnes[], int max) {
    int nb = 0;
    char *p = ligne;

    colonnes[nb++] = p;

    while (*p != '\0' && nb < max) {
        if (*p == ';') {
            *p = '\0';
            colonnes[nb++] = p + 1;
        }

        if (*p == '\n') {
            *p = '\0';
            break;
        }

        p++;
    }

    return nb;
}
