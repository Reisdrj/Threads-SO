#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>


int max = 5;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int ready = 0; 

void *incrementfunc(void *arg){
    pthread_mutex_lock(&mutex);
    while(!ready){
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);
    while ((max) < 10)
    {
        max++;
        printf("%s: %d\n", (char *)arg, max);
    }
    pthread_exit(arg);
}

void *decrementfunc(void *arg){
    pthread_mutex_lock(&mutex);
    while(!ready){
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);
    while ((max) > 0)
    {
        max--;
        printf("%s: %d\n", (char *)arg, max);
    }
    pthread_exit(arg);
}

void* signal_thread(void* arg) {
    sleep(1);

    pthread_mutex_lock(&mutex);
    ready = 1;
    printf("Signaling all waiting threads.\n");
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(){

    pthread_t increment;
    pthread_t decrement;
    pthread_t disparo;
    void * thread_res;
    int rstatus;
    
    /*tenta iniciar o thread, indicando a função 'routine' e passando como argumento a string "Thread A"*/
    rstatus = pthread_create (&increment, NULL, incrementfunc, (void*)("Thread Increment"));
    if(rstatus != 0){
        printf("Thread increment não instanciada!!!\n");
    }

    rstatus = pthread_create (&decrement, NULL, decrementfunc, (void*)("Thread Decrement"));

    if(rstatus != 0){
        printf("Thread decrement não instanciada!!!\n");
    }

    pthread_create(&disparo, NULL, signal_thread, NULL);   
    pthread_join(disparo, NULL);
    return 0;
}