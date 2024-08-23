/* CMPS 3600
   Lab Assignment #08
   Erik Soto
   Lab 08: The Racers Problem
*/

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

int R0 = 0;
int R1 = 0;

pthread_mutex_t B0;
pthread_mutex_t B1;

void *bridge0(void *a){
    do{
        if (R1 == 20000)
            pthread_exit(NULL);

        while (!pthread_mutex_trylock(&B0)){}
        pthread_mutex_trylock(&B1);
        R0++;
        pthread_mutex_unlock(&B0);
        pthread_mutex_unlock(&B1);

    } while (R0 != 20000);
    pthread_exit(NULL);
}

void *bridge1(void *a){
    do{
        if (R0 == 20000)
            pthread_exit(NULL);
        while (!pthread_mutex_trylock(&B1)){}
        pthread_mutex_trylock(&B0);
        R1++;
        pthread_mutex_unlock(&B0);
        pthread_mutex_unlock(&B1);

    } while (R1 != 20000);
    pthread_exit(NULL);
}

int main(){

    pthread_t tid[2];

    pthread_mutex_init(&B0, NULL);
    pthread_mutex_init(&B1, NULL);
    pthread_create(&tid[0], NULL, bridge0, NULL);
    pthread_create(&tid[1], NULL, bridge1, NULL);
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    printf("Total Laps:\n");
    printf("R0: %d\tR1: %d\n", R0, R1);
    if(R0 == 20000)
        printf("R0 is the winner!\n");
    else
        printf("R1 is the winner!\n");
}
