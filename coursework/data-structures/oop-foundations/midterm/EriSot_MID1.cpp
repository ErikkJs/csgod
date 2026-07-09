#include <iostream>

using namespace std;

class Degree{
    private:
        string _desc;
        int _years;
    public:
        Degree( string d ,int y) 
    {
        _desc = d;
        _years = y; 
    }

        string get_desc()
        {
            return _desc;
        }

        int get_years()
        {
            return _years;
        }

        void show()
        {
            cout << get_desc() << " takes " << get_years() << " years to complete" << endl;
        }

};


int main()
{
    Degree d("art" , 4);

    d.show();



    return 0;
}

