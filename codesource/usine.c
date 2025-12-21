#include <stdlib.h>
#include "usine.h"

/*/////////////////////////////////////////////////////////////////////////*/
/*     usine.c possède 2 fonction : la première chaine_vers_int            */
/*     permettant de passer de chaine à entier (volume d'eau) et la        */
/*     deuxieme permettant de passer de chaine à un nombre à vigule        */
/*     (pourcentage de fuite).                                             */
/*/////////////////////////////////////////////////////////////////////////*/

int chaine_vers_int(char *s) {
    int r = 0;

    if (s == NULL) {
        return 0;
    }

    for (int i = 0; s[i] >= '0' && s[i] <= '9'; i++) {
        r = r * 10 + (s[i] - '0');
    }

    return r;
}

float chaine_vers_float(char *s) {
    if (s == NULL) {
        return 0.0;
    }

    float r = 0.0;
    float div = 1.0;
    int dec = 0;

    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] == '.') {
            dec = 1;
        }
        else if (s[i] >= '0' && s[i] <= '9') {
            r = r * 10 + (s[i] - '0');
            if (dec) {
                div *= 10.0;
            }
        }
        else {
            break;
        }
    }

    return r / div;
}

