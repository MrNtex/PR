#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include"pomiar_czasu.h"

int zmienna_globalna=0;

int main(){
  
  int pid, wynik, i;

  inicjuj_czas();
  
  for(i=0;i<1000;i++){
    
    pid = fork(); 			
    
    if(pid==0){ 			

      /*
      char arg1[] = "/bin/ls";
      char arg2[] = ".";
      char* arg[] = {arg1,arg2,NULL};
      wynik=execv("./program",arg); 
      if(wynik==-1)
        printf("Proces potomny nie wykonal programu\n");
      */

      exit(0);
      
    } else {					
      zmienna_globalna++; 
      wait(NULL);
      
    }
    
  }

  drukuj_czas();
  printf("Wartosc zmiennej globalnej po utworzeniu 1000 procesow: %d\n", zmienna_globalna);
  
}

  
