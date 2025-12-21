#ifndef USINE_H
#define USINE_H

typedef struct {
    char *id;
    int vol_max;
    int vol_src;
    int vol_real;
} Usine;

int chaine_vers_int(char *s);
float chaine_vers_float(char *s);

#endif
