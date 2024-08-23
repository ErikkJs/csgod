#include <iostream>
#include <stdio.h>
using namespace std;

int main(){

    int myInt;
    myInt = 0xFFFFFFF;
    for(int i=myInt; i >= 0; i+=1){
        printf("%d\n",i);
    }
    return 0;
}

