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

  zmienna_globalna++;
  
  FILE* fptr;
  fptr = fopen("test.txt", "r");

  if (fptr == NULL)
	  printf("Plik nie zostal otwarty.");

  return 0;
}

int thread_create_file(void* argument)
{
  FILE* fptr;
  fptr = fopen("test.txt", "w");

  if (fptr == NULL)
    printf("Plik nie otwarty");
  else
    printf("Plik utworzono pomyslnie.");
  return 0;
}

int main()
{

  void *stos, *stos2;
  pid_t pid;
  pid_t pid2;
  int i; 
  
  inicjuj_czas();
  
  stos = malloc( ROZMIAR_STOSU );
  stos2 = malloc(ROZMIAR_STOSU);
  if (stos == 0) {
    printf("Proces nadrzędny - blad alokacji stosu\n");
    exit( 1 );
  }

  pid = clone(&thread_create_file, (void*) stos+ROZMIAR_STOSU, SIGCHLD, 0);
  pid2 = clone(&funkcja_watku, (void*) stos2+ROZMIAR_STOSU, SIGCHLD, 0);

  waitpid(pid, NULL, __WCLONE);
  waitpid(pid2, NULL, __WCLONE);

  for(i=0;i<1000;i++){
    // clone jak fork
    pid = clone( &funkcja_watku, (void *) stos+ROZMIAR_STOSU, 
      SIGCHLD, 0 );

    waitpid(pid, NULL, __WCLONE);

  }

  drukuj_czas();
  printf("Wartosc zmiennej globalnej po utworzeniu 1000 watkow: %d\n", zmienna_globalna);

  free( stos );
}
