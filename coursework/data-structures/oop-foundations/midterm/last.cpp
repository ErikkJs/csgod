#include <iostream>

using namespace std;

bool split( string key, string &value)
{
    int equals = line.find('=');

    key= line.substr(0, equals);

    value = line.substr(equals +1, line.length() - equals);

    return true;

}

int main()
{
    string value;

    if( split("name-castor", "name" , value))
    {
        cout << "value: " << value << endl;
        cout << "key = " << value << endl;
    }

    ifstream.ifs("config.ini");
    if ( ifs.open())
    {
        while(!ifs.eof())
        {
            ifs>> line;
            if(ifs.good())
            {
                split(line, key, value);
                cout << "key = " << key << endl;
                cout << "value = " << value << endl;
                cout << "-----------" << endl;
            }
        }
    }

        ifs.close();

    return 0;
}

