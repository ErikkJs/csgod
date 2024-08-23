#include <iostream>

using namespace std;

int main()
{
    int a;

    cout << "enter a number";
    cin>> a;

    if( a & 2)
        cout << "got 2" << endl;

    if( a & 4)
        cout << "got 4" << endl;
    if( a & 8)
        cout << "got 8" << endl;

    if( a & 16)
        cout << " got 16 " << endl;

return 0;
}

// & = bitwise and
// | = bitwise or
