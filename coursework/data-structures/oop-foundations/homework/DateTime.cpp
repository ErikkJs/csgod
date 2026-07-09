#include <iostream>

using namespace std;

class DateTime{
    private:
        int _month, _day, _year;
    public:
        string to_text(char = '/');
        friend bool operator<(const DateTime &, const DateTime&);


        DateTime(int m , int d , int y)
        {
           _month = m;
           _day = d;
           _year = y;
        }

};
bool operator <(const DateTime &lhs, const DateTime & rhs )
{
    return( lhs._year < rhs._year)

        &&(lhs._month < rhs._month)

        &&(lhs._day < rhs._day);


}


string DateTime::to_text(char c)
{
    // compile with g++std=c++11
    return to_string(_month) + c + to_string(_day) + c + to_string(_year);

}


int main()
{

    DateTime d1(6, 21, 2016);
    DateTime christmas(4, 25,2018);

    if( d1< christmas)
        cout << d1.to_text() << " is older " << christmas.to_text()<< endl;
    else 
        cout << d1.to_text() << " is after " << christmas.to_text() << endl;



    return 0;
}

