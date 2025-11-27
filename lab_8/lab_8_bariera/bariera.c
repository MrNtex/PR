#include <pthread.h>
#include <stdio.h>
#include "bariera.h"

void bariera_init(bariera_t* b, int n) {
    b->liczba_watkow = n;
    b->licznik_oczekujacych = 0;
    b->id_bariery = 0;

    pthread_mutex_init(&b->mutex, NULL);
    pthread_cond_init(&b->cond, NULL);
}

void bariera(bariera_t* b) {
    pthread_mutex_lock(&b->mutex);
    int moje_id_bariery = b->id_bariery;
    
    b->licznik_oczekujacych++;

    if (b->licznik_oczekujacych == b->liczba_watkow) {
        b->licznik_oczekujacych = 0;

        b->id_bariery++;

        pthread_cond_broadcast(&b->cond);
        
    } else {
        while (moje_id_bariery == b->id_bariery) {
            pthread_cond_wait(&b->cond, &b->mutex);
        }
    }

    pthread_mutex_unlock(&b->mutex);
}