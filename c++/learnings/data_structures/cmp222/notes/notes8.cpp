#include <iostream>

using namespace std;

class A{
    private:
        int _data1, _data2;
        int *_pdata;
    public:
        A(){
            _pdata = new int;
        }
        A(const A &a)
        {
            cout << "Copy constructor" << endl;
        }

        A& operator = (const & A&a) // if one parameter "this" is the left hand side 
        {
            cout << "assignment operator" << endl;
        }


        int main ()
        {
            A a1, a2, a3;
            a3 = a1; /// MEMBERWISE ASSIGNMENT

            A a4 = a1;
            return 0;

        }
        
