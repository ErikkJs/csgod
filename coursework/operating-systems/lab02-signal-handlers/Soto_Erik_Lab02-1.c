#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

using namespace std; 

bool fax =1;
void handler(int signum){
    printf("Hello World!\n");
    fax = false;
}

int main(int argc, char * argv[])
{
    signal(SIGALRM,handler);
    alarm(1);
   
while(200){
    alarm(1);
    sleep(1);
    printf("Turing was right!\n");
    }
    return 0;
}
