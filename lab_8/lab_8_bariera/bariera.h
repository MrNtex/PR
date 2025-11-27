#include <pthread.h>

typedef struct {
    int liczba_watkow;
    int licznik_oczekujacych;
    int id_bariery;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} bariera_t;