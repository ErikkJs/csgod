#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
using namespace std;

double rollf() {
    double f = (rand()/RAND_MAX);
    return f;
}

int rolli(int n) {
    int i = (rand()%(n-1));
    return i;
}

void initPop(int pop, int n) {
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < n; ++j) {
            if ((rand()/(RAND_MAX)) >= 0.5) {
                pop[i][j] = 1;
            }
            else {
                pop[i][j] = 0;
            }
        }
    }
}

void findFitness(int pop, int fitness, int n) {
    for (int i = 0; i < 10; ++i) {
        fitness[i] = 0;
        for (int j = 0; j < n; ++j) {
            fitness[i] += pop[j][i];
        }
    }
}

void crossover(int pop, int fitness, int n) {
    int probC = 1;
    bool nextGen[10][2*n];
    int genFitness[2*n];
    for (int i = 0; i < n; ++i) {

}

void mutate(int pop, int n) {
    double probM = 0.01;
    for (int i = 0; i < n; ++i) {
        if ((rand()/(RAND_MAX)) <= probM) {
            pop[i][j] = !pop[i][j];
        }
    }
}

int selection(int pop, int fitness, int n) {
    int i = rolli(n);
    return i;
}

int tournament(int n) {
    int roll, parent;
    int max = -1;

    for (int i = 0; i < n; ++i) {
        roll = rolli(POP_SIZE);
        if (fitness[roll] > max) {
            max = fitness[roll];
            parent = roll;
        }
    }
    return parent;
}

int main(){
    const int GENE_SIZE = 10;
    const int POP_SIZE = 10
    int pop[GENE_SIZE][POP_SIZE];
    int fitness[POP_SIZE];
    srand(time(0));
    
    //create population and find each individuals fitness
    initPop(pop, n);
    findFitness(pop, fitness, n);




return 0;
}
