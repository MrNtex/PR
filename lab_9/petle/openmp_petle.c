#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define WYMIAR 10

int main ()
{
    double a[WYMIAR][WYMIAR];
    int i, j;

    for(i=0;i<WYMIAR;i++) for(j=0;j<WYMIAR;j++) a[i][j]=1.02*i+1.01*j;

    double suma=0.0;
    for(i=0;i<WYMIAR;i++) {
        for(j=0;j<WYMIAR;j++) {
            suma += a[i][j];
        }
    }
    printf("Suma wyrazow tablicy (sekwencyjnie): %lf\n\n", suma);

    printf("--- WARIANT 1: Dekompozycja wierszowa (schedule static, 2) ---\n");
    
    double suma_parallel = 0.0;

#pragma omp parallel for default(none) shared(a) private(j) reduction(+:suma_parallel) schedule(static, 2) ordered
    for(i=0; i<WYMIAR; i++) {
        #pragma omp ordered
        {
            for(j=0; j<WYMIAR; j++) {
                suma_parallel += a[i][j];
                printf("(%1d,%1d)-W_%1d ", i, j, omp_get_thread_num());
            }
            printf("\n");
        }
    }
    printf("Suma rownolegle (Wariant 1): %lf\n\n", suma_parallel);

    printf("--- WARIANT 2: Dekompozycja kolumnowa wewnetrzna (schedule dynamic) ---\n");
    
    suma_parallel = 0.0;

    for(i=0; i<WYMIAR; i++) {
#pragma omp parallel for default(none) shared(a, i) reduction(+:suma_parallel) schedule(dynamic) ordered
        for(j=0; j<WYMIAR; j++) {
            suma_parallel += a[i][j];
            
            #pragma omp ordered
            printf("(%1d,%1d)-W_%1d ", i, j, omp_get_thread_num());
        }
        printf("\n");
    }
    printf("Suma rownolegle (Wariant 2): %lf\n\n", suma_parallel);


    printf("--- WARIANT 3: Zamiana petli (j zewnetrzna, schedule static) ---\n");

    suma_parallel = 0.0;

#pragma omp parallel default(none) shared(a, suma_parallel) private(i, j)
    {
        double suma_prywatna = 0.0;

#pragma omp for schedule(static) ordered
        for(j=0; j<WYMIAR; j++) {
#pragma omp ordered
            for(i=0; i<WYMIAR; i++) {
                suma_prywatna += a[i][j];
                printf("(%1d,%1d)-W_%1d ", i, j, omp_get_thread_num());
            }
            printf("\n");
        }

#pragma omp critical
        {
            suma_parallel += suma_prywatna;
        }
    }
    printf("Suma rownolegle (Wariant 3): %lf\n", suma_parallel);

    printf("--- WARIANT 4: Dekompozycja 2D (blokowa 2x2, nested) ---\n");
    
    omp_set_nested(1);
    
    suma_parallel = 0.0;

    omp_set_num_threads(3);

#pragma omp parallel for schedule(static, 2) shared(a, suma_parallel) private(j)
    for(i=0; i<WYMIAR; i++) {
        int id_outer = omp_get_thread_num();
        double suma_prywatna = 0.0;
        omp_set_num_threads(2);

#pragma omp parallel for schedule(static, 2) firstprivate(id_outer) reduction(+:suma_prywatna) ordered
        for(j=0; j<WYMIAR; j++) {
            suma_prywatna += a[i][j];

            #pragma omp ordered
            {
                printf("(%1d,%1d)-W_(%d,%d) ", i, j, id_outer, omp_get_thread_num());
            }
        }
#pragma omp critical
        {
            suma_parallel += suma_prywatna;
            printf("\n");
        }
    }
    printf("Suma rownolegle (Wariant 4): %lf\n\n", suma_parallel);


    printf("--- WARIANT 5: Dekompozycja 2D (duże bloki, nested, static bez porcji) ---\n");
    
    suma_parallel = 0.0;
    
    omp_set_num_threads(3);

#pragma omp parallel for schedule(static) shared(a, suma_parallel) private(j)
    for(i=0; i<WYMIAR; i++) {
        
        int id_outer = omp_get_thread_num();
        double suma_prywatna = 0.0;
        omp_set_num_threads(2);

#pragma omp parallel for schedule(static) firstprivate(id_outer) reduction(+:suma_prywatna) ordered
        for(j=0; j<WYMIAR; j++) {
            suma_prywatna += a[i][j];

            #pragma omp ordered
            {
                printf("(%1d,%1d)-W_(%d,%d) ", i, j, id_outer, omp_get_thread_num());
            }
        }
        #pragma omp critical
        {
            suma_parallel += suma_prywatna;
            printf("\n");
        }
    }
    printf("Suma rownolegle (Wariant 5): %lf\n", suma_parallel);

    return 0;
}