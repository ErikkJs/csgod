#include <iostream>

using namespace std;

class Time {
    protected: 
        int hours;
        int minutes;
        int seconds;
    public:
        Time(int h, int m, int s):
            hours(h), minutes(m) , seconds(s)
    {
    }
};
class Date{
    protected:
        int month ;
        int day;
        int year;

    public:
        Date (int m, int d, int y):
            month(m) , day(d) , year(y)
    {}
        int get_month() {return month;}
        int get_day() {return day;}
        int get_year() { return year;}
};

class TimeZone{
    protected:
        string zone_name;
        int utc_offset;
};

class DateTime : public Date , public Time , public TimeZone{
    public:
        DateTime(int m , int d, int y)
            : Date(m,d,y) , Time(0,0,0)
        {}

        DateTime (int mm , int d, int y, int h , int m, int s):
            Date(mm,d,y), Time(h,m,s)
    {}
        void show_date()
        {
            cout << get_month << "/" << get_day() << "/" << get_year() << endl;
        }
            void show()
            {
                cout << month <<
            }

};


int main()
{

    DateTime d1(2,18,2016);
    d1.show_date();

    DateTime d2(2,18,2016,5,0,0);
    d2.show();

    return 0;


}
