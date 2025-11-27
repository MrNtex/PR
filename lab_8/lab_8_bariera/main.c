#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include "bariera.h"

extern void bariera_init(bariera_t*, int);
extern void bariera(bariera_t*);

#define LICZBA_W 4
#define LICZBA_BARIER 3
//#define LICZBA_W 44

typedef struct {
  int id;
  int n_barier;
  bariera_t *tablica_barier;
} argumenty_t;

pthread_t watki[LICZBA_W];

void *cokolwiek( void *arg);

int main( int argc, char *argv[] ){

  int i, indeksy[LICZBA_W]; for(i=0;i<LICZBA_W;i++) indeksy[i]=i; 

  // use of pthread_once (once_control, init_routine) ?
  // int pthread_once(pthread_once_t *once_control, void (*init_routine)(void));
  // but init_routine has no arguments...
  bariera_t moje_bariery[LICZBA_BARIER];

  for(int k = 0; k < LICZBA_BARIER; k++) {
      bariera_init(&moje_bariery[k], LICZBA_W);
  }

  argumenty_t args[LICZBA_W];

  for(i=0; i<LICZBA_W; i++ ) {
    args[i].id = i;
    args[i].n_barier = LICZBA_BARIER;
    args[i].tablica_barier = moje_bariery;
    pthread_create( &watki[i], NULL, cokolwiek, (void *) &args[i] );
  }

  for(i=0; i<LICZBA_W; i++ ) pthread_join( watki[i], NULL );

  pthread_exit( NULL);  
}

void *cokolwiek(void *arg){

    argumenty_t *dane = (argumenty_t *)arg;
    int moj_id = dane->id;
    int n = dane->n_barier;
    
    bariera_t *tablica = dane->tablica_barier;

    printf("Watek %d startuje\n", moj_id);

    for(int k = 0; k < n; k++) {
        printf("Watek %d: robie zadanie nr %d...\n", moj_id, k+1);
        bariera(&tablica[k]);
        printf("Watek %d: PRZESZEDL bariere nr %d\n", moj_id, k+1);
    }

    printf("Watek %d: Koniec pracy.\n", moj_id); 

    pthread_exit((void *)0);
}
