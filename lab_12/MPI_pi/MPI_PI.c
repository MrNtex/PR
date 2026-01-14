#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define SCALAR double
//#define SCALAR float

#ifndef M_PI // standardy C99 i C11 nie wymagajÄ definiowania staĹej M_PI
#define M_PI (3.14159265358979323846)
#endif

int main( int argc, char *argv[] ){ // program obliczania przybliĹźenia PI za pomocÄ wzoru Leibniza
                  // PI = 4 * ( 1 - 1/3 + 1/5 - 1/7 + 1/9 - 1/11 + itd. )
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int max_liczba_wyrazow=0;

  if(rank==0){
    printf("Podaj maksymalnÄ liczbÄ wyrazĂłw do obliczenia przybliĹźenia PI\n");
    scanf("%d", &max_liczba_wyrazow);
  }

  MPI_Bcast(&max_liczba_wyrazow, 1, MPI_INT, 0, MPI_COMM_WORLD);
  SCALAR suma_plus=0.0;
  SCALAR suma_minus=0.0;

  int my_start = (rank * max_liczba_wyrazow) / size;
  int my_end = ((rank + 1) * max_liczba_wyrazow) / size;
  int i=0;
  for(i=my_start; i<my_end; i++){  
    int j = 1 + 4*i; 
    suma_plus += 1.0/j;
    suma_minus += 1.0/(j+2.0);
    //printf("PI obliczone: %20.15lf, aktualna poprawka: %20.15lf\n",
    //  	 4*(suma_plus-suma_minus), 4.0/j-4.0/(j+2.0));
  }
  
  SCALAR global_suma_plus = 0.0;
  SCALAR global_suma_minus = 0.0;

  MPI_Reduce(&suma_plus, &global_suma_plus, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Reduce(&suma_minus, &global_suma_minus, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
      double pi_approx = 4 * (global_suma_plus - global_suma_minus);
    //  t = MPI_Wtime() - t;
    printf("PI obliczone: \t\t\t%20.15lf\n", pi_approx);
    printf("PI z biblioteki matematycznej: \t%20.15lf\n", M_PI);
    printf("Blad bezwzgledny: \t\t%20.15lf\n", fabs(pi_approx - M_PI));
    //  printf("Czas obliczeĹ: %lf\n", t);
  }

  MPI_Finalize();
  return 0;
  
}

