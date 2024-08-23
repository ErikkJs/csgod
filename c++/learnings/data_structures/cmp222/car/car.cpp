#include "car.h"
#include <iostream>

using namespace std;

Car::Car(string make, string model, int year)
{
    _make =make;
    _model = model;
    _year =year;
}

Car::Car(string make,string model)
{
    _make = make;
    _model= model;
    _year = 2016;

}
Car::string get_make()
{
    return _make;
}
Car:: string get_model()
{
    return _model;
}

Car:: int get_year()
{
    return _year;
}

