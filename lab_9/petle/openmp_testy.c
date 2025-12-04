#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define WYMIAR 18

int main ()
{
    double a[WYMIAR];
    // Inicjalizacja danych
    for(int i=0;i<WYMIAR;i++) a[i]=1.02*i;

    printf("=== TEST 1: schedule(static, 3) ===\n");
    #pragma omp parallel for default(none) shared(a) num_threads(4) schedule(static, 3)
    for(int i=0; i<WYMIAR; i++) {
        int id_w = omp_get_thread_num();
#pragma omp ordered
      printf("a[%2d]->W_%1d  \n",i,id_w); 
    }

    printf("\n=== TEST 2: schedule(static) [domyślny rozmiar] ===\n");
    #pragma omp parallel for default(none) shared(a) num_threads(4) schedule(static)
    for(int i=0; i<WYMIAR; i++) {
        int id_w = omp_get_thread_num();
#pragma omp ordered
      printf("a[%2d]->W_%1d  \n",i,id_w); 
    }

    printf("\n=== TEST 3: schedule(dynamic, 2) ===\n");
    #pragma omp parallel for default(none) shared(a) num_threads(4) schedule(dynamic, 2)
    for(int i=0; i<WYMIAR; i++) {
        int id_w = omp_get_thread_num();
#pragma omp ordered
      printf("a[%2d]->W_%1d  \n",i,id_w); 
    }

    printf("\n=== TEST 4: schedule(dynamic) [domyślny rozmiar = 1] ===\n");
    #pragma omp parallel for default(none) shared(a) num_threads(4) schedule(dynamic)
    for(int i=0; i<WYMIAR; i++) {
        int id_w = omp_get_thread_num();
#pragma omp ordered
      printf("a[%2d]->W_%1d  \n",i,id_w); 
    }

    return 0;
}