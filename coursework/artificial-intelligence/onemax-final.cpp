#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sstream>
/*
 * Erik Soto
 * Dr. Cruz
 * 4/23/17
 * cmps3560
 * lab8
   */
double pm=0.01;
double pc=.5;
int n=10;
bool pop[10][10];
int fitness[10];

void crossover(bool pop[10][10], int fitness[10]);
void mutate(bool pop[10][10]);
void fit();
int selection(int n);
double rollf();
int rolli(int n);
void initPop(bool pop[10][10], int fitness[10]);

using namespace std;

int main()
{

    int temp=0, t=0;
    srand(unsigned(time(NULL)));

    initPop(pop, fitness);
    fit();

    do{
        t++;
        cout << endl;
        cout << "*********************************************************************" << endl;
        for(int i=0; i<10; i++) {
            cout << "Individual " << i+1 << ": ["; 
            for(int j=0; j<10; j++){
                cout << pop[i][j];
            } 
            cout << "] and fitness is " << fitness[i] << endl;
            if(fitness[i] > temp)
                temp = fitness[i];
        } 
            if(rollf()<=pc)
                crossover(pop, fitness);
        fit();

        cout << "*********************************************************************" << endl;
        if(temp != 10){
            cout << "Generation " << t << " Best indivual's fitness is " << temp << ", " << endl;
            cout << "continuing with next genereation" << endl;
            cout << endl;
        }
        else{
            cout << "Generation " << t << " Best indivual's fitness is " << temp << ", " << endl;
            cout << "we are finished!!!" << endl;
            cout << endl;
        }
    }while(temp!=10);


    return(0);
}

void crossover(bool pop[10][10], int fitness[10]){
    int c1, c2;
    bool temp[10][10];
    do {
    c1 = selection(n);
    c2 = selection(n);
    } while(c1 == c2);

    for(int i=0; i<10; i++) {
        for(int j=0; j<10; j++){
            temp[i][j] = pop[i][j];
        }
    }

    for(int j=0; j<10; j++){
        if(j>5){
            pop[c1][j] = temp[c2][j];
            pop[c2][j] = temp[c1][j];
        }
    }
    mutate(pop);
}

void fit() {
    for(int i=0; i<10; i++) {
        fitness[i] = 0;
        for(int j=0; j<10; j++) {
            fitness[i] += pop[i][j];
        }
    }
}
int selection(int n) {
    int max = -1, roll, parent;

    for(int i=0; i<n; i++) {
        roll = rolli(10);
        if(fitness[roll]>max) {
            max = fitness[roll];
            parent = roll;
        }
    }
    return parent;
}

void mutate(bool pop[10][10]) {
    for(int i=0; i<10; i++) {
        for(int j=0; j<10; j++){
            if(rollf() <= pm){
                pop[i][j] ^= 1;
            }
        } 
    }
} 


void initPop(bool pop[10][10], int fitness[10]) {
    int r = rand()%2;
    for(int i=0; i<10; i++) {
        for(int j=0; j<10; j++){
            r = rand()%2;
            pop[i][j] = r;
        }
    }
}
double rollf() {
    //double r = rand()%100+1;
    double r = ((double)rand()/(RAND_MAX));
    return r;
}
//this needs to be called in selection for the random number index that is going to be used inside the crossover function
int rolli(int n) {
    int r = rand()%n+1;
    return r;
}

