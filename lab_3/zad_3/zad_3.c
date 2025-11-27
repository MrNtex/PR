#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include <unistd.h>

#define ILOSC_WATKOW 4

struct Calka {
  // IN
  double a;
  double b;

  // IN/OUT
  double wynik;
};

void* funkcja_watku(void* arg){
    struct Calka* calka = (struct Calka*)arg;

    calka->wynik = (calka->a + calka->b) / 2.0;
    printf("Watek potomny: Przedzial [%f, %f], Wynik %f\n", calka->a, calka->b, calka->wynik);
    return NULL;
}

int main()
{
  struct Calka calka[ILOSC_WATKOW];
  pthread_t threads[ILOSC_WATKOW];

  double a = 0.0;
  double b = 10.0;
  double h = (b - a) / ILOSC_WATKOW;

  for (int i = 0; i < ILOSC_WATKOW; i++) {
      calka[i].a = a + i * h;
      calka[i].b = calka[i].a + h;
      calka[i].wynik = -1.0;

      pthread_create(&threads[i], NULL, funkcja_watku, &calka[i]);
  }

  for (int i = 0; i < ILOSC_WATKOW; i++) {
      pthread_join(threads[i], NULL);
  }

  double wynik = 0.0;
  for (int i = 0; i < ILOSC_WATKOW; i++) {
      wynik += calka[i].wynik;
  }

  printf("Wynik calkowania: %f\n", wynik);
}