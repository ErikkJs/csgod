//erik soto
//3-3-16
//lab6

#include <iostream>

using namespace std;

class MyExceptionBase
{
    private:
        // error message
        string _text;
    public:
        // constructor sets the text
        MyExceptionBase(string t)
        {
            _text = t;

        }
        // returns the error
        string get_error()
        {
            return _text;
        }
};

// handles more specific errors of type MyException
class MyExceptionChild : public MyExceptionBase
{
    private: 
        string _text;
    public:
        MyExceptionChild(string t)
            : MyExceptionBase(t) {}
};

int main()
{
    try
    { throw 1;

       // throw MyExceptionBase("oops");
    }
   
    catch(MyExceptionChild &ex)
    {
        cerr << "Child handler handled it\n";
        cerr << ex.get_error() << endl;
    }
    catch(MyExceptionBase &ex)
    {  
        cerr << "Base handler handled it\n";
        cerr << ex.get_error() << endl;
    }
    catch(...)
    {
        cerr << "aww, snap,hes dead jim" << endl;
    }


    return 0;
}


