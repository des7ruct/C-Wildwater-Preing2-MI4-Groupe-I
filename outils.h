#ifndef UTILS_H
#define UTILS_H
#include <time.h>

int valeur_absente(char *a);
char *trim(char *s);
int decouper_ligne(char *ligne, char *colonnes[], int max);
long temps_en_millisecondes(struct timespec debut, struct timespec fin);

#endif
