#ifndef CAR_H
#define Car_H
using namespace std;

class Car{
    private:
        string _make;
        string _model;
        int _year;
    public:
        // constructor that uses the variables
        Car(string ,string ,int);
        Car(string , string );
        string get_make();
        string get_model();
        int get_year();

};

#endif
