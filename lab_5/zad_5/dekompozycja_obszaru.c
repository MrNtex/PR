#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>

typedef struct {
    double a_local;
    double b_local;
    double dx;
} DaneWatku;

double funkcja ( double x );

double calka_dekompozycja_obszaru(double a, double b, double dx, int l_w);

void* calka_podobszar_w(void* arg_wsk);

double calka_dekompozycja_obszaru(double a, double b, double dx, int l_w){
  pthread_t watki[l_w];
  DaneWatku dane_watkow[l_w];

  //printf("a %lf, b %lf, dx %lf\n", a, b, dx);

  double calka_suma_local = 0.0;

  // tworzenie struktur danych do obsługi wielowątkowości

  double przedzial = (b - a) / l_w;

  for (int i = 0; i < l_w; i++) {
      dane_watkow[i].a_local = a + i * przedzial;
      dane_watkow[i].b_local = (i == l_w - 1) ? b : dane_watkow[i].a_local + przedzial;
      dane_watkow[i].dx = dx;

      pthread_create(&watki[i], NULL, calka_podobszar_w, (void*)&dane_watkow[i]);
  }

  // oczekiwanie na zakończenie pracy wątków
  for (int i = 0; i < l_w; i++) {
      double* wynik;
      pthread_join(watki[i], (void**)&wynik);
      calka_suma_local += *wynik;
      free(wynik);
  }

  return(calka_suma_local);
}



void* calka_podobszar_w(void* arg_wsk){

  DaneWatku* dane = (DaneWatku*)arg_wsk;
  double a = dane->a_local;
  double b = dane->b_local;
  double dx      = dane->dx;

  double* wynik = malloc( sizeof(double) );

  int N_local = ceil((b - a) / dx);
  double dx_adj = (b - a) / N_local;
  double calka = 0.0;
  double f_1 = funkcja(a);
  for (int i = 0; i < N_local; i++) {
      double x1 = a + i * dx_adj;
      double f_2 = funkcja(x1 + dx_adj);
      calka += 0.5 * dx_adj * (f_1 + f_2);
      f_1 = f_2;
  }

  *wynik = calka;
  pthread_exit((void*)wynik);
}
