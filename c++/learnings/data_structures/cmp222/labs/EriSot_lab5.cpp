// Erik Soto
// Lab5
// Feb 16 2016

#include <iostream>

using namespace std;

class Employee{
    protected: 
        double PTO_accrual; // PTO accrual rate
        double PTO_accrued; // accrued PTO
        double hours; // hours worked in cycle 
        double OT; // OT hours in cyccle 
        double emp_rate;
    public: 
        virtual void get_hours(double rate) =0;
       // gets the pto and adds to the overall pto
        virtual void get_PTO_accrued()
        {
            double number;

            cout << " enter the PTO" << endl;
            cin >> number;

            PTO_accrued = PTO_accrual + number;
        }
        // shows the pay 
        void show_pay()
        {
            double pay;

            pay =  ( hours  * emp_rate ) + (OT * emp_rate *1.5);

            cout << " Your Pay Is: " <<  pay << " Your PTO is : " << PTO_accrued;
        }

};

class Exempt : public  Employee{
    public:
        Exempt()
        {

            PTO_accrual = 4.66;

        }

        // gets the hours worked , saves the rate in the protected variable
        void get_hours(double rate) 
        {

            emp_rate = rate;
            OT = 0;

            double hours_worked;
            cout << " Enter hours worked " << endl;
            cin >> hours_worked;

            hours = hours_worked;

        }
};

class NonExempt : public Employee{
    public:
        NonExempt()
        {
            PTO_accrual = 3.16;
        }

    // saves the variables , gets the overtime as well as normal hours worked
        void get_hours(double rate)
        {
            double OT_hours;
            double hours_worked;
            emp_rate = rate;  

            cout << " Enter hours worked" << endl;
            cin >> hours_worked;

            cout << " Enter OT hours worked" << endl;
            cin >> OT_hours;

            hours = hours_worked;
            OT = OT_hours;
        }

};



// menu to get the choice from the user
char get_choice()
{
    char c;

    cout << " (A) : Exempt" << endl;
    cout << " (B) : Non-Exempt" << endl;
    cin >> c;

    return c;
}



int main()
{
    Employee *a;

    // checks to see at runtime what you are creating 
    switch(get_choice())
    {
        case 'a' : a =  new Exempt(); break;
        case 'b': a = new NonExempt(); break;
    }

    a->get_hours(15);
    a->get_PTO_accrued();
    a->show_pay();

    delete a;
    return 0;

}

