// Erik Soto
// lab1
// 1/12/16

#include <iostream>

using namespace std;

class Contact {
    private:
        string _first;
        string _last;
        string _company;

        void clear()
        {
            _first = "";
            _last = "";
            _company = "";
        }

    public:
        string get_display_name()
        {
            string u = "unknown";
            string f =  _first;
            string l = _last;

            if( _first != "" && _last != "")
            {
                return f + l;

            }
            else if (_company != "")
            {
                return _company;
            }

            else 
                return u;
        }

};
void Contact::Contact()
{
    clear();
}

void Contact::Contact(string c)
{   
    clear();

    _company = c;

}
void Contact::Contact(string f, string l)
{
    clear();

    _first = f;
    _last = l;
}

int main ()
{

    Contact Contact *c1 , *c2 , *c3;
    *c1 = new int;
  cout <<  Contact *c1() << endl;
 cout << Contact *c2("Erik" ,"Soto"); << endl;
cout << Contact *c3("Apple") << endl;

delete *c1;
delete *c2;
delete *c3;




    return 0;

}


