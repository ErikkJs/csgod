#include <iostream>

using namespace std;

int safediv(int x , int y)
{
    if ( y=0)
    // one way >>    cerr << "divide by 0" << endl;  
    // cerr is a error message thingy , 
                                         // cin / cout family ^^
        throw 1;
        // nothing after the "throw" will be executed
        // forced murder
    return x/y
}


int main()
{
    int y;
    cout << " y: "; 
    cin >> y;

    try {
        cout << safediv(5,y) << endl;
        cout << "done\n";
    }

    catch (...)// three dots means "catch all" catch all of the errors
    {
        cout << "failed. exception caught \n";
    }

    cout << "program done \n";
       
    
    }

catch(char *)
{
    cerr << "failed. exception caught\n" ;
}


    return 0;
}

