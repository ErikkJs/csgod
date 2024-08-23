// erik soto
// lab 2
// jan 19 2016

#include <iostream>

using namespace std;

class Session {
    private:
        string _location;
        bool _loggedin;
    public:
        static int live_sessions;
        Session(string s , bool f);
        Session();
        void logout()
        {
            live_sessions --;
        }
        friend void show(Session &s); 
};

void show(Session &s)
{
    //looks to see if there is no locations 
    if( s._location == "")
    {
        cout << "Not logged in " << "Sessions:" << s.live_sessions << endl;


    }
    else 
        // shows locations where someone has logged in and the number of sessions
        cout << "logged in at "<<  s._location << " Sessions: " << s.live_sessions << endl;
}


int Session::live_sessions = 0;

// sets the location and logged in status to blanks
Session::Session()
{
    _location = "";
    _loggedin = false;

}
//passes in the parameters to the variables
Session::Session(string s , bool f)
{
    _location = s;
    _loggedin = f;

        // if login was successful add to live_sessions
    if(_loggedin == true)
    {
        live_sessions++;
    }

}

int main()
{

    Session s1;
    show(s1);

    Session s2("Phone", true);
    show(s2);

    Session s3("Computer", true);
    show(s3);

    Session s4("Tablet", true);
    show(s4);

    s4.logout();
    s3.logout();
    show(s4);

    return 0;

}

