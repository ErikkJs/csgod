#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
using namespace std; 

bool fax =1;
int sigs=0;

clock_t start;

void handler(int signum){
    printf("Hello World!\n");
    fax = false;
    sigs++;
}
void handlerINT(int signum){

    clock_t endtime=clock(); 
    clock_t cloks = endtime - start;
    double duration = cloks/ (double)CLOCKS_PER_SEC; 
    // duration = (clock()-start)/(double) CLOCKS_PER_SEC; 
    cout << endl << "You Sent " << sigs << " signals" << endl;
    cout << "You Spent " << duration << " seconds" << endl;

    exit(1);
}
void handlerSIGFPE(int sig)
{  
    cout << "MATH ERROR YOU CANT DO THAT " << endl;
    kill(getpid(),SIGKILL);
}
void fill(int *arr)
{
    srand(time(NULL));
    for(int i =0; i < 5; i++)
    {
        int ran = rand()% 20+1;
        arr[i] = ran;
    }
}
int divide(int n1, int n2){

    printf("Number 1: %d \n",n1);
    printf("Number 2: %d \n",n2);

    return n1/n2;

}

int main(int argc, char * argv[])
{
    int res,num1,num2;
    srand(time(NULL));
    int ran = rand() % 5+1;

    int ar[5];
    int br[5];
    fill(ar);
    fill(br);
    br[ran]=0; 



    start = clock();

    signal(SIGALRM,handler);
    signal(SIGINT,handlerINT);
    signal(SIGFPE,handlerSIGFPE);
    alarm(1);

    while(200){
        clock_t s = start;
        sleep(1); 
        ran = rand() % 5+1;
        num1 = ar[ran]; 
        ran = rand() % 5+1;
        num2 = ar[ran];
        res = divide(num1,num2);
        alarm(0);
        printf("Turing was right!\n");
    }
    return 0;
}
