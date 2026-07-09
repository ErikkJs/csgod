//// MILES PER GALLON


#include <iostream>
using namespace std;


int calculator(int,int);

int main ()
{
    int gallons;
    int miles;

    cout << " how many gallons does your car hold" << endl;
    cin >>gallons;

    cout << " how many miles can it be driven on a full tank of gas? " << endl;
    cin >> miles;

cout << " your car can drive " << calculator(gallons, miles) << "miles per gallon" << endl;

}

int calculator(int miles,int gallons)
{

    int calc; 
    calc = gallons/ miles;

    return calc;
}
