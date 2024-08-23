#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

int main()
{
    srand(time(0));
    int random;
    random = rand()%100;


    cout << random<< endl;

    return 0;
}

