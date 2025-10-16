#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>


void* funkcja_watku(void* arg) {
    pthread_attr_t attr;
    pthread_getattr_np(pthread_self(), &attr);

    size_t stacksize, guardsize;
    void* stackaddr;
    int detachstate, scope, inheritsched, schedpolicy;
    struct sched_param schedparam;

    pthread_attr_getstack(&attr, &stackaddr, &stacksize);
    pthread_attr_getguardsize(&attr, &guardsize);
    pthread_attr_getdetachstate(&attr, &detachstate);
    pthread_attr_getscope(&attr, &scope);
    pthread_attr_getinheritsched(&attr, &inheritsched);
    pthread_attr_getschedpolicy(&attr, &schedpolicy);
    pthread_attr_getschedparam(&attr, &schedparam);

    printf("\n==============================\n");
    printf("WATEK (PID %ld)\n", pthread_self());
    printf("==============================\n");
    printf("Adres stosu: %p\n", stackaddr);
    printf("Rozmiar stosu: %zu bajtow\n", stacksize);
    printf("Guard size: %zu bajtow\n", guardsize);
    printf("Detach state: %s\n", detachstate == PTHREAD_CREATE_JOINABLE ? "JOINABLE" : "DETACHED");
    printf("Scope: %s\n", scope == PTHREAD_SCOPE_SYSTEM ? "SYSTEM" : "PROCESS");
    printf("Inherit sched: %s\n", inheritsched == PTHREAD_INHERIT_SCHED ? "INHERIT" : "EXPLICIT");

    const char* policy_str =
        (schedpolicy == SCHED_OTHER) ? "SCHED_OTHER" :
        (schedpolicy == SCHED_FIFO) ? "SCHED_FIFO" :
        (schedpolicy == SCHED_RR)   ? "SCHED_RR" : "UNKNOWN";
    printf("Sched policy: %s\n", policy_str);
    printf("Sched priority: %d\n", schedparam.sched_priority);

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    pthread_getaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

    printf("Affinity (rdzenie): ");
    for (int i = 0; i < CPU_SETSIZE; i++) {
        if (CPU_ISSET(i, &cpuset))
            printf("%d ", i);
    }
    printf("\n");

    pthread_attr_destroy(&attr);
    return NULL;
}

int main()
{
  pthread_t watki[3];
  pthread_attr_t attr;

  pthread_create(&watki[0], NULL, funkcja_watku, NULL);

  pthread_attr_init(&attr);
  pthread_attr_setstacksize(&attr, 1024*1024);
  pthread_create(&watki[1], &attr, funkcja_watku, NULL);
  pthread_attr_destroy(&attr);

  pthread_attr_init(&attr);
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(0, &cpuset); 
  pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
  pthread_create(&watki[2], &attr, funkcja_watku, NULL);
  pthread_attr_destroy(&attr);

  for (int i = 0; i < 3; i++) {
      pthread_join(watki[i], NULL);
  }
}