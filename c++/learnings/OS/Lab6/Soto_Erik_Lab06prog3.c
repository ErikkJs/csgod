//ErikSoto
//CMPS 3600
// Lab6
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define ITERAT 1000000

int TOTAL = 0;
pthread_mutex_t mutex;


void * Count(void * a){
    int n = *(int*)a;
    pthread_mutex_lock(&mutex);
    TOTAL++;
    printf("threadID= %d\ti= %d\ttmp= %d\n",pthread_self(), n, TOTAL);
    sleep(1);
        
    pthread_mutex_unlock(&mutex);
    
}

int main(int argc, char * argv[]){

    pthread_t tid[10];
    

    for(int i = 0; i < 10; i++){
        if(pthread_create(&tid[i], NULL, Count, &i)){
            printf("\n ERROR creating thread 1");            
            exit(1);
        }
    }
    

    for(int i = 0; i < 10; i++){
        if(pthread_join(tid[i], NULL))	/* wait for the thread 1 to finish */
        {
            printf("\n ERROR joining thread");
            exit(1);
        }
    }
    

  
    pthread_exit(NULL);
}

