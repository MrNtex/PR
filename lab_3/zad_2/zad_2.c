#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include <unistd.h>

#define ILOSC_WATKOW 5

void* funkcja_watku(void* arg){
    printf("Watek potomny: PID %ld\n", pthread_self());
    printf("Watek potomny: Argument %d\n", *(int*)arg);
    return NULL;
}

int main()
{
  pthread_t threads[ILOSC_WATKOW];
  //int thread_ids[ILOSC_WATKOW];

  for (int i = 0; i < ILOSC_WATKOW; i++) {
      //thread_ids[i] = i;
      pthread_create(&threads[i], NULL, funkcja_watku, &i);
  }

  for (int i = 0; i < ILOSC_WATKOW; i++) {
      pthread_join(threads[i], NULL);
  }
}