#include <stdlib.h>
#include<stdio.h>
#include<math.h>
#include<unistd.h>

#include "mpi.h"

#define HOSTNAME_MAX_LENGTH 256

int main( int argc, char** argv ){ 
  
  int rank, ranksent, size, source, dest, i; 
  MPI_Status status;
  char hostname[HOSTNAME_MAX_LENGTH];
  char recv_hostname[HOSTNAME_MAX_LENGTH];
  int tag_rank = 0;
  int tag_hostname = 1;
  
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank ); 
  MPI_Comm_size( MPI_COMM_WORLD, &size );

  gethostname(hostname, HOSTNAME_MAX_LENGTH);
  
  if(size>1){
    
    if( rank != 0 ){ dest=0;
      
      MPI_Send( &rank, 1, MPI_INT, dest, tag_rank, MPI_COMM_WORLD );
      MPI_Send( hostname, HOSTNAME_MAX_LENGTH, MPI_CHAR, dest, tag_hostname, MPI_COMM_WORLD );
      
    } else {
      
      for( i=1; i<size; i++ ) { 
	
	MPI_Recv( &ranksent, 1, MPI_INT, MPI_ANY_SOURCE, 
		  tag_rank, MPI_COMM_WORLD, &status );
      int sender_rank = status.MPI_SOURCE;

      MPI_Recv( recv_hostname, HOSTNAME_MAX_LENGTH, MPI_CHAR, sender_rank,
        tag_hostname, MPI_COMM_WORLD, &status );
	printf("Dane od procesu o randze (status.MPI_SOURCE ->) %d: %d uruchomionego na wezle %s (i=%d)\n", 
	       status.MPI_SOURCE, ranksent, recv_hostname, i );
      }
      
    }

  }
  else{
	printf("Pojedynczy proces o randze: %d (brak komunikatów)\n", rank);
  }

  
  MPI_Finalize(); 
  
  return(0);

}

