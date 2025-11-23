#include <pthread.h>
#include <stdio.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static int licznik_oczekujacych = 0;
static int liczba_watkow = 0;
static int id_bariery = 0;

void bariera_init(int n) {
    liczba_watkow = n;
    licznik_oczekujacych = 0;
    id_bariery = 0;
}

void bariera(void) {
    pthread_mutex_lock(&mutex);
    int moje_id_bariery = id_bariery;
    
    licznik_oczekujacych++;

    if (licznik_oczekujacych == liczba_watkow) {
        licznik_oczekujacych = 0;

        id_bariery++;

        pthread_cond_broadcast(&cond);
        
    } else {
        while (moje_id_bariery == id_bariery) {
            pthread_cond_wait(&cond, &mutex);
        }
    }

    pthread_mutex_unlock(&mutex);
}