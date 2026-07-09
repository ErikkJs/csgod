#include <iostream>

using namespace std;

class Date {
    private:    
    int month;
    int day;
    int year;

   public:
    Date(int m , int d, int y);
    void show();


};

Date::Date()
{
    _month = 1;
    _day = 1;
    _year = 1500;
}


void Date::date(int m, int d, int y)
{
    _month = m;
    _day = d;
    _year = y;
}

void Date::show()
{
    cout << _month << "/" << _day << "/" << _year;

}

int main ()
{


return 0;
}

