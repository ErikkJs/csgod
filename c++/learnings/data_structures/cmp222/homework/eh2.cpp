#include <iostream>


using namespace std;

int safediv( int x , int y)
{
    if ( y==0)
        throw 1;

    return x/y;
}


void C()
{
    cout << "c-start" << endl;
    safediv(3,0);
    cout << "c-end" << endl;
}

void B()
{
    cout << "b-start" << endl;
    try{
    C();
    }

    catch(int)
    {}

    cout << "C-End" << endl;
}


void A ()
{
    cout << "A-start" << endl;
    B();

    try{
    B();
    }

    catch(int)
    {}

    cout << "A-end" << endl;
}


int main()
{
 cout << "Main-start" <<endl;
 A();
 cout << "main- end" << endl;

 return 0;

}

