#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

// #define ZAMKNIETY_PIERSCIEN 1 

int main(int argc, char** argv) {
  int rank, size;
  int value = 0;
  int tag = 0;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int next = (rank + 1) % size;
  int prev = (rank - 1 + size) % size;

  if (rank == 0) {
    value = 100;
    
    printf("Proces %d (START) wysłał liczbę %d do procesu %d\n", rank, value, next);
    MPI_Send(&value, 1, MPI_INT, next, tag, MPI_COMM_WORLD);

    #ifdef ZAMKNIETY_PIERSCIEN
        MPI_Recv(&value, 1, MPI_INT, prev, tag, MPI_COMM_WORLD, &status);
        printf("Proces %d odebrał liczbę %d od procesu %d (PĘTLA ZAMKNIĘTA)\n", rank, value, prev);
    #endif
  }
  else {
    MPI_Recv(&value, 1, MPI_INT, prev, tag, MPI_COMM_WORLD, &status);
    printf("Proces %d odebrał liczbę %d od procesu %d\n", rank, value, prev);
    value += 1; 

    if (rank == size - 1) {
      #ifdef ZAMKNIETY_PIERSCIEN
        printf("Proces %d wysłał liczbę %d do procesu %d (zamykanie pętli)\n", rank, value, next);
        MPI_Send(&value, 1, MPI_INT, next, tag, MPI_COMM_WORLD);
      #else
        printf("Proces %d kończy pierscien z liczbą %d. (BRAK WYSYŁKI)\n", rank, value);
      #endif
    } 
      else {
        printf("Proces %d wysłał liczbę %d do procesu %d\n", rank, value, next);
        MPI_Send(&value, 1, MPI_INT, next, tag, MPI_COMM_WORLD);
      }
  }

  MPI_Finalize();
  return 0;
}