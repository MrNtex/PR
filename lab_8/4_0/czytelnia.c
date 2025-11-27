#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"czytelnia.h"


/*** Implementacja procedur interfejsu ***/

int my_read_lock_lock(cz_t* cz_p){
  pthread_rwlock_rdlock(&cz_p->rw_lock);

  pthread_mutex_lock(&cz_p->licznik_mutex);
  cz_p->l_c++;
  pthread_mutex_unlock(&cz_p->licznik_mutex);
}


int my_read_lock_unlock(cz_t* cz_p){
   
  pthread_mutex_lock(&cz_p->licznik_mutex);
  cz_p->l_c--;
  pthread_mutex_unlock(&cz_p->licznik_mutex);
  pthread_rwlock_unlock(&cz_p->rw_lock);
}


int my_write_lock_lock(cz_t* cz_p){
  
  pthread_rwlock_wrlock(&cz_p->rw_lock);

  pthread_mutex_lock(&cz_p->licznik_mutex);
  cz_p->l_p++;
  pthread_mutex_unlock(&cz_p->licznik_mutex);
}


int my_write_lock_unlock(cz_t* cz_p){
  pthread_mutex_lock(&cz_p->licznik_mutex);
  cz_p->l_p--;
  pthread_mutex_unlock(&cz_p->licznik_mutex);
  pthread_rwlock_unlock(&cz_p->rw_lock);
}

void inicjuj(cz_t* cz_p){

  cz_p->l_p = 0;
  cz_p->l_c = 0;  

  pthread_rwlock_init(&cz_p->rw_lock, NULL);
  pthread_mutex_init(&cz_p->licznik_mutex, NULL);
}

void czytam(cz_t* cz_p){

// wypisanie wartości zmiennych kontrolujących działanie: liczby czytelników i pisarzy
  printf("\t\t\t\t\tczytam:  l_c %d, l_p %d\n", cz_p->l_c, cz_p->l_p); 
// sprawdzenie warunku poprawności i ewentualny exit
// warunek:
  if( cz_p->l_p>1 || (cz_p->l_p==1 && cz_p->l_c>0) || cz_p->l_p<0 || cz_p->l_c<0 ) {
    printf("Blad: ....\n");
    //exit(0);
  }

    usleep(rand()%3000000);
}

void pisze(cz_t* cz_p){

// wypisanie wartości zmiennych kontrolujących działanie: liczby czytelników i pisarzy
  printf("\t\t\t\t\tpisze:   l_c %d, l_p %d\n", cz_p->l_c, cz_p->l_p); 
// sprawdzenie warunku poprawności i ewentualny exit
  if( cz_p->l_p>1 || (cz_p->l_p==1 && cz_p->l_c>0) || cz_p->l_p<0 || cz_p->l_c<0 ) {
    printf("Blad: ....\n");
    //exit(0);
  }

    usleep(rand()%3000000);
}


