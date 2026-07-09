#include <iostream>
#include <fstream>

using namespace std;

enum DoorState{LOCKED , UNLOCKED, DISABLED};
class Door {
    private:

        int _MAX_ATTEMPTS = 3;
        string name;
        DoorState _state;
        string _codes[10];

        void load();

    public:
        Door();
        DoorState access(string code);
};

DoorState Door::access(string code)
{
    _state = LOCKED;
    load();
    for( int p =0; p <3; p++)
    {

        for ( int i=0; i <10; i++)
        {
            string line;

            line = code;

            if ( line == _codes[i])
            {
                _state =UNLOCKED;
                return _state;
            }

        }  
        cout << p;

        if( p ==3)
        {
            _state =DISABLED;
            return _state;
        }

    }




}

void Door::load()
{
    ifstream ifs;
    ifs.open("codes.txt");

    string line;
    int i =0;
    if (ifs.is_open())
    {

        while (!ifs.eof())
        {
            getline(ifs, line);

            if(ifs.good())
            {
                _codes[i] = line;
                i++;
            }
        }
    }

    ifs.close();

}



Door::Door()
{
    _state = LOCKED;
    load();
    _MAX_ATTEMPTS =3;
}


int main()
{

    Door door1;
    DoorState state;
    string user_code = "";
    while (true)
    {

        cout << "enter code:";
        cin >> user_code;

        state = door1.access(user_code);

        while (state == LOCKED || state == DISABLED)
        {
            cout << "enter code:";
                cin >> user_code;

            state= door1.access(user_code);
        }

        return 0;

    }
}

