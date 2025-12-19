#include <time.h>
#include <stdio.h>


*/////////////////////////////////////////////////////////////*/
*/         Exemple de timer possible en mili secondes         */
*/////////////////////////////////////////////////////////////*/


long temps_en_millisecondes(struct timespec debut, struct timespec fin) {
    long secondes = fin.tv_sec - debut.tv_sec;
    long nanosecondes = fin.tv_nsec - debut.tv_nsec;
    return secondes * 1000 + nanosecondes / 1000000;
}

int main(void) {
    struct timespec debut, fin;

    clock_gettime(CLOCK_MONOTONIC, &debut);

    /* -------------------------
       TON CODE ICI
       ------------------------- */

    clock_gettime(CLOCK_MONOTONIC, &fin);

    long duree_ms = temps_en_millisecondes(debut, fin);
    printf("Temps d'exécution : %ld ms\n", duree_ms);

    return 0;
}
