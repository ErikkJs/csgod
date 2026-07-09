#include <iostream>
#include "car.h"

using namespace std;

int main()
{
    Car car1("Tesla","Model S", 2017);
    Car car2("Faraday Future" , "FFZERO1");
    
    cout << car1.get_make() << car1.get_model << car1.get_year << endl;  
    cout << car2.get_make() << car2.get_model << car2.get_year << endl;
  
    return 0;
}

