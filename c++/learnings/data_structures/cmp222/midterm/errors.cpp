#include <iostream>
#include <iostream>

using namespace std;

int main ()
{
    int i;

    cout << " enter a number" << endl;
    cin >> i;

    assert(i==0);  // assert takes a bull value  // perfect for debugging
    // checks to see if i == 0; 
    cout << "all ok" << endl;


    return 0;
}

