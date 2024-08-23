#include <iostream>

using namespace std;

int * create_int(int num1)
{
    int *num = new int;
    *num = num1;
    return num;
}

double * create_int(double num1)
{
    double *num = new double;
    *num = num1;
    return num;
}

int main ()
{
    // create an int ptr
    int *num = (create_int(10));
    double *yeet =(create_int(11.5));
    cout << *num<< endl;
    cout << *yeet << endl;

    delete num;
    return 0;
}

