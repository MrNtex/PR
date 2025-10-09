#define _GNU_SOURCE
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <linux/sched.h>

#include "pomiar_czasu.h"

int zmienna_globalna=0;

#define ROZMIAR_STOSU 1024*64

int funkcja_watku( void* argument )
{
  int* local = (int*) argument;

  for(int i = 0; i < 1000000; i++) {
    zmienna_globalna++;
    (*local)++;
  }
    
  printf("Watek (PID %d) zakonczyl: lokalna = %d, globalna = %d\n",
           getpid(), *local, zmienna_globalna);

  return 0;
}

int main()
{

  void *stos, *stos2;
  pid_t pid, pid2;
  int lokalna1 = 0, lokalna2 = 0;
  
  inicjuj_czas();
  
  stos = malloc( ROZMIAR_STOSU );
  stos2 = malloc( ROZMIAR_STOSU );
  if (stos == 0 || stos2 == 0) {
    printf("Proces nadrzędny - blad alokacji stosu\n");
    exit( 1 );
  }

  pid = clone( &funkcja_watku, (void *) stos+ROZMIAR_STOSU, 
		 CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, &lokalna1 );
  pid2 = clone( &funkcja_watku, (void *) stos2+ROZMIAR_STOSU, 
		 CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, &lokalna2 );

  waitpid(pid, NULL, __WCLONE);
  waitpid(pid2, NULL, __WCLONE);

  drukuj_czas();
  printf("Po zakonczeniu watkow: lokalna1 = %d, lokalna2 = %d, globalna = %d\n", lokalna1, lokalna2, zmienna_globalna);

  free( stos );
  free( stos2 );
}
