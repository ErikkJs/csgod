#include <iostream>
#include "animal.h"

using namespace std;

int main()
{
    Animal a("Dog" , CARNIVORE);
    Animal g("Mountain Goat" , HERBIVORE);

    cout << a.get_name() << endl;
    cout <<g.get_name() << endl;

    return 0;
}

