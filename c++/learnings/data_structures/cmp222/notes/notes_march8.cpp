#include <iostream>

using namespace std;


template <typename T> // template , makes all the "T"s the passed in value
T multiply(T x , T y)
{
    return x * y;
}

// sepecialization 
template <>
void show<double>(double value)
{
    cout << scientific << value << endl;
}


template <typename T>
void show(T value)
{
    cout << value << endl;
}


int multiply( int x, int y)
{
    return x*y;
}

double multiply( double x , double y)
{
    return x * y;
}

int main()
{
    // intergers
    cout << multiply(3,5) << endl;

    // automatically casting to interger

    cout << multiply(3.0,1.5) << endl;

    return 0;

}

