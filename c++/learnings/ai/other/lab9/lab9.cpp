#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <limits>
using namespace std;

void initParent(double parent[], int coeff) {
    for (int i = 0; i < coeff; ++i) {
        parent[i] = (rand()*(40/RAND_MAX)-20);
    }
}

double noise(double mu, double sigma) {
    const double epsilon = std::numeric_limits<double>::min();
    const double twoPi = 2.0*3.14159265358979323846;
    
    static double num0, num1;
    static bool gen;
    gen = !gen;

    if (!gen) return num1 * sigma + mu;

    double rand1, rand2;
    do {
        rand1 = rand() * (1.0 / RAND_MAX);
        rand2 = rand() * (1.0 / RAND_MAX);
    } while ( rand1 <= epsilon );
    
    num0 = sqrt(-2.0 * log(u1)) * cos(twoPi * u2);
    num1 = sqrt(-2.0 * log(u1)) * sin(twoPi * u2);
    return num0 * sigma + mu;
}

void mutate(double parent[], double offspring[],  int coeff) {
    for (int i = 0; i < coeff; ++i) {
        offspring[i] = parent[i] + noise(0,1);
    }
}

double fitness(double off[], double y[], double x[]) {
    double fit = 0;
    for (int i = 0; i < N; ++i) {
        fit += abs(y[i]-(off[0]*pow(x[i],3)+off[1]*pow(x[i],2)+off[2]*x[i]+off[3]);
    }
}
