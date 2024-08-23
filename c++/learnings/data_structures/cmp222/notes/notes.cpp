#include <iostream>

using namespace std;


enum Suit { SPADE , DIAMOND , CLUB , HEART};
enum Day { SUNDAY, MONDAY, TUESDAY};
enum Month { JAN , FEB , APR};
enum Loginstatus { UNKNOWN, LOGGEDIN, LOGEDOUT, LOCKOUT, WAITING_VERIFY};
enum FacebookRelationship { UNSET, SINGLE, MARRIED, ITS_COMPLICATED};

string show_fb_rel(FacebookRelationship);


int main ()

{

    Loginstatus ls;
    FacebookRelationship fb;

    if ( ls ==UNKNOWN)

        cout << "login not read"<< endl;
    return 0;
}

string show_fb_rel()
{

    switch (fb)
    {
        case UNSET: return "";
        case SINGLE:return "Single";
        case MARRIED: return "married";
        case ITS_COMPLICATED: return "its _complicated";
                              default return "";

    }

}
















int main ( int argc , char *argv[])
{
    if (argc ==1)
    { 
        cout << " No parameters given" << endl;
       
        return 0;
    }

    else 
    {
        for (int i =0; i <argc; i++)
        {
            cout << " parameter" << i + 1 << "I" << argv [i] << endl;
        }
    }

    return 0;

}






















