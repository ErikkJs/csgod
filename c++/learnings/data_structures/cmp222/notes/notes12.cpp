#include <iostream>
#include "animal.h"

using namespace std;

Animal::Animal(string name, Diet diet)
{
    _name = name;
    _diet =diet;
}

string Animal::get_name()
{

    return _name;

}

