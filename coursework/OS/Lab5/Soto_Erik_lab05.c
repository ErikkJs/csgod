/*
CMPS 3600
Erik Soto
Lab 05
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include <pthread.h>
#include <semaphore.h>

void *output(void *arg);
sem_t mysemap;


int code[] = { 4, 6, 3, 1, 5, 0, 2 };

int main(){
  pthread_t tid[7];
  sem_init(&mysemap, 0, 1);

  //sort code[] 0 to 6
  int i;
  int j;
  int n = 7;
  for( i = 0; i < n-1; i++){
    for( j = 0; j < n-i-1; j++){
      if(code[j] > code[j+1]){
        int temp = code[j];
        code[j] = code[j+1];
        code[j+1] = temp;
      }
    }
  }
  
  for (i = 0; i < 7; i++){
    sem_wait(&mysemap);
    pthread_create(&tid[i], NULL, output, (void*)&code[i]);
    
  }

  for (i = 0; i < 7; i++)
    pthread_join(tid[i], NULL);

  sem_destroy(&mysemap);

  return 0;
}

void *output(void *arg){  
  
  int n = *(int*)arg;
  switch (n)
  {
    case 0:
      printf("a) A semaphore S is an integer-valued variable which can take only non-negative\n");
      printf("values. Exactly two operations are defined on a semaphore:\n\n");
      break;

    case 1:
      printf("b) Signal(S): If there are processes that have been suspended on this semaphore,\n");
      printf(" wake one of them, else S := S+1.\n\n");
      break;

    case 2:
      printf("c) Wait(S): If S>0 then S:=S-1, else suspend the execution of this process.\n");
      printf(" The process is said to be suspended on the semaphore S.\n\n");
      break;

    case 3:
      printf("d) The semaphore has the following properties:\n\n");
      break;

    case 4:
      printf("1. Signal(S) and Wait(S) are atomic instructions. In particular, no\n");
      printf(" instructions can be interleaved between the test that S>0 and the\n");
      printf(" decrement of S or the suspension of the calling process.\n\n");
      break;

    case 5:
      printf("2. A semaphore must be given an non-negative initial value.\n\n");
      break;

    case 6:
      printf("3. The Signal(S) operation must waken one of the suspended processes. The\n");
      printf(" definition does not specify which process will be awakened.\n\n");
      break;
  }
  sem_post(&mysemap);

  pthread_exit(0);
}

