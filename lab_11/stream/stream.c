#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mpi.h"

#define MAX_STR 100

typedef struct {
  int id;
  char text[MAX_STR]; 
  int length;
  int a_count;
} WorkPacket;

void create_mpi_struct_type(MPI_Datatype *mpi_work_packet_type) {
  int blocklengths[4] = {1, MAX_STR, 1, 1};
  MPI_Aint displacements[4];
  MPI_Datatype types[4] = {MPI_INT, MPI_CHAR, MPI_INT, MPI_INT};

  MPI_Aint base_address;
  WorkPacket dummy;
  MPI_Get_address(&dummy, &base_address);
  MPI_Get_address(&dummy.id, &displacements[0]);
  MPI_Get_address(&dummy.text, &displacements[1]);
  MPI_Get_address(&dummy.length, &displacements[2]);
  MPI_Get_address(&dummy.a_count, &displacements[3]);

  for(int i=0; i<4; i++) displacements[i] -= base_address;

  MPI_Type_create_struct(4, blocklengths, displacements, types, mpi_work_packet_type);
  MPI_Type_commit(mpi_work_packet_type);
}

int main(int argc, char** argv) {
  int rank, size;
  MPI_Status status;
  MPI_Datatype mpi_work_packet_type;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  create_mpi_struct_type(&mpi_work_packet_type);

  int next = rank + 1;

  if (rank == 0) {
    const char* sentences[] = {
      "Ala ma kota",
      "Systemy Rownolegle",
      "Message Passing Interface",
      "Test potoku",
      "Koniec zdan na dzis albo tez nie"
    };
    int num_sentences = 5;

    for (int i = 0; i < num_sentences; i++) {
      WorkPacket packet;
      packet.id = i + 1;
      strcpy(packet.text, sentences[i]);
      packet.length = 0;
      packet.a_count = 0;

      printf("[Gen %d] Wpuszczam do potoku: '%s'\n", rank, packet.text);
      MPI_Send(&packet, 1, mpi_work_packet_type, next, 0, MPI_COMM_WORLD);
    }

    WorkPacket end_packet;
    end_packet.id = -1;
    MPI_Send(&end_packet, 1, mpi_work_packet_type, next, 0, MPI_COMM_WORLD);
    printf("[Gen %d] Zakonczono generowanie.\n", rank);
  }
  
  else {
    WorkPacket packet;
    int prev = rank - 1;

    while (1) {
      MPI_Recv(&packet, 1, mpi_work_packet_type, prev, 0, MPI_COMM_WORLD, &status);

      if (packet.id == -1) {
        if (rank < size - 1) {
          MPI_Send(&packet, 1, mpi_work_packet_type, next, 0, MPI_COMM_WORLD);
        }
        printf("[Proc %d] STOP.\n", rank);
        break;
      }
      
      if (rank == 1) {
        packet.length = strlen(packet.text);
      }
      else if (rank == 2) {
        int count = 0;
        for(int i=0; packet.text[i] != '\0'; i++) {
          if(packet.text[i] == 'a' || packet.text[i] == 'A') count++;
        }
        packet.a_count = count;
      }
      else if (rank == 3) {
        for(int i=0; packet.text[i] != '\0'; i++) {
          packet.text[i] = toupper(packet.text[i]);
        }
      }

      if (rank < size - 1) {
        MPI_Send(&packet, 1, mpi_work_packet_type, next, 0, MPI_COMM_WORLD);
      } else {
        printf(" -> [FINAL %d] Pakiet #%d: Text='%s', Len=%d, 'A'=%d\n", 
                rank, packet.id, packet.text, packet.length, packet.a_count);
      }
    }
  }


  MPI_Type_free(&mpi_work_packet_type);
  MPI_Finalize();
  return 0;
}