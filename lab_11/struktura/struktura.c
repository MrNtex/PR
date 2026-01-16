#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

#define BUF_SIZE 1000

typedef struct {
    int licznik;
    double wartosc;
    char imie[50];
} MojaStruktura;

int main(int argc, char** argv) {
    int rank, size;
    int tag = 0;
    MPI_Status status;
    
    char buffer[BUF_SIZE];
    int position = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;

    MojaStruktura dane;

    if (rank == 0) {
        dane.licznik = 1;
        dane.wartosc = 3.1415;
        strcpy(dane.imie, "Artur"); 

        printf("[Proces %d] INICJUJE: ID=%d, Val=%.4f, Imie=%s\n", 
               rank, dane.licznik, dane.wartosc, dane.imie);

        position = 0;
        MPI_Pack(&dane.licznik, 1, MPI_INT, buffer, BUF_SIZE, &position, MPI_COMM_WORLD);
        MPI_Pack(&dane.wartosc, 1, MPI_DOUBLE, buffer, BUF_SIZE, &position, MPI_COMM_WORLD);
        int len = strlen(dane.imie) + 1;
        MPI_Pack(&len, 1, MPI_INT, buffer, BUF_SIZE, &position, MPI_COMM_WORLD);
        MPI_Pack(dane.imie, len, MPI_CHAR, buffer, BUF_SIZE, &position, MPI_COMM_WORLD);

        MPI_Send(buffer, position, MPI_PACKED, next, tag, MPI_COMM_WORLD);
        MPI_Recv(buffer, BUF_SIZE, MPI_PACKED, prev, tag, MPI_COMM_WORLD, &status);

        position = 0;
        MPI_Unpack(buffer, BUF_SIZE, &position, &dane.licznik, 1, MPI_INT, MPI_COMM_WORLD);
        printf("[Proces %d] OTRZYMAL ZWROT: ID=%d\n", rank, dane.licznik);
    }
    else {
        MPI_Recv(buffer, BUF_SIZE, MPI_PACKED, prev, tag, MPI_COMM_WORLD, &status);
        position = 0;
        MPI_Unpack(buffer, BUF_SIZE, &position, &dane.licznik, 1, MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(buffer, BUF_SIZE, &position, &dane.wartosc, 1, MPI_DOUBLE, MPI_COMM_WORLD);
        
        int len;
        MPI_Unpack(buffer, BUF_SIZE, &position, &len, 1, MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(buffer, BUF_SIZE, &position, dane.imie, len, MPI_CHAR, MPI_COMM_WORLD);
        printf(" -> [Proces %d] Odebral: ID=%d, Val=%.4f, Imie=%s\n", 
               rank, dane.licznik, dane.wartosc, dane.imie);
        dane.licznik++;
        dane.wartosc += 1.1;
        position = 0;
        MPI_Pack(&dane.licznik, 1, MPI_INT, buffer, BUF_SIZE, &position, MPI_COMM_WORLD);
        MPI_Pack(&dane.wartosc, 1, MPI_DOUBLE, buffer, BUF_SIZE, &position, MPI_COMM_WORLD);
        
        len = strlen(dane.imie) + 1;
        MPI_Pack(&len, 1, MPI_INT, buffer, BUF_SIZE, &position, MPI_COMM_WORLD);
        MPI_Pack(dane.imie, len, MPI_CHAR, buffer, BUF_SIZE, &position, MPI_COMM_WORLD);

        printf(" -> [Proces %d] Wysyla ID=%d\n", rank, dane.licznik);
        MPI_Send(buffer, position, MPI_PACKED, next, tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}