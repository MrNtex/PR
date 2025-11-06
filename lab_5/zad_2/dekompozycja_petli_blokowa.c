#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>

double funkcja ( double x );

double calka_zrownoleglenie_petli(double a, double b, double dx, int l_w);

static int l_w_global=0;

static double calka_global=0.0;
static double a_global;
static double b_global;
static double dx_global;
static int N_global;

// mutex do bezpiecznej redukcji wielowątkowej
static pthread_mutex_t calka_mutex = PTHREAD_MUTEX_INITIALIZER;

void* calka_fragment_petli_w(void* arg_wsk);

double calka_zrownoleglenie_petli(double a, double b, double dx, int l_w){

  int N = ceil((b-a)/dx);
  double dx_adjust = (b-a)/N;

  printf("Obliczona liczba trapezów: N = %d, dx_adjust = %lf\n", N, dx_adjust);
  //printf("a %lf, b %lf, n %d, dx %.12lf (dx_adjust %.12lf)\n", a, b, N, dx, dx_adjust);


  // tworzenie struktur danych do obsługi wielowątkowości
  calka_global = 0.0;
  a_global = a;
  b_global = b;
  dx_global = dx_adjust;
  N_global = N;
  l_w_global = l_w;

  pthread_t *watki = (pthread_t*)malloc(sizeof(pthread_t) * l_w);
  int *ids = (int*)malloc(sizeof(int) * l_w);
  if(!watki || !ids){
    fprintf(stderr, "Błąd alokacji pamięci dla wątków.\n");
    free(watki); free(ids);
    return 0.0;
  }


  // tworzenie wątków
  for(int i=0; i<l_w; ++i){
    ids[i] = i;
    pthread_create(&watki[i], NULL, calka_fragment_petli_w, (void*)&ids[i]);
  }


  // oczekiwanie na zakończenie pracy wątków
  for(int i=0; i<l_w; ++i){
    pthread_join(watki[i], NULL);
  }

  free(ids);
  free(watki);


  return(calka_global);
}

void* calka_fragment_petli_w(void* arg_wsk){

  int my_id = *(int*)arg_wsk;

  double a, b, dx; // skąd pobrać dane a, b, dx, N, l_w ? 
  int N, l_w;      // wariant 1 - globalne

  // wartości globalne nadaje calka_zrownoleglenie_petli
  a = a_global;
  b = b_global;
  dx = dx_global;
  N = N_global;
  l_w = l_w_global;

  // dekompozycja cykliczna
  // int my_start = my_id;     // start od identyfikatora wątku
  // int my_end = N;           // do N (nie włącznie)
  // int my_stride = l_w;      // skok równy liczbie wątków

  // dekompozycja blokowa
  //int my_start = 0;
  //int my_end = 0;
  //int my_stride = 0;

  // Aby przetestować wariant blokowy, zakomentuj powyższe 3 linie (cykliczne)
  // i odkomentuj poniższy fragment. Zapewnia równomierny podział (ostatni blok może być krótszy).
  int blok = (N + l_w - 1) / l_w;   // rozmiar bloku (zaokrąglanie w górę)
  int my_start = my_id * blok;
  int my_end = my_start + blok; if(my_end > N) my_end = N;
  int my_stride = 1;

  // something else ? (dekompozycja blokowo-cykliczna)

  printf("\nWątek %d: my_start %d, my_end %d, my_stride %d\n", 
	 my_id, my_start, my_end, my_stride);


  int i;
  double calka = 0.0;
  for(i=my_start; i<my_end; i+=my_stride){

    double x1 = a + i*dx;
    calka += 0.5*dx*(funkcja(x1)+funkcja(x1+dx));
    //printf("i %d, x1 %lf, funkcja(x1) %lf, całka = %.15lf\n", 
    //	   i, x1, funkcja(x1), calka);

  }

  // Redukcja wielowątkowa do zmiennej wspólnej – ochrona mutexem
  pthread_mutex_lock(&calka_mutex);
  calka_global += calka;
  pthread_mutex_unlock(&calka_mutex);

  return NULL;
}

