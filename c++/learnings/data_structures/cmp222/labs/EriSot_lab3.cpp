//erik soto
//lab 3
//1/25/16

#include <iostream>

using namespace std;

class Week{
    private:
        int _num;
        double *_hours;
    public:

        //makes the array
        Week(int size)
       {
           _hours = new double[size];
       }
            // fill the array
        void fill(double value)
        {
            for(int i =0; i < 5; i++)
            {    
               this-> _hours[i] = value;
            }
        }

            //sets the num variable to the parameter
        void set_num(int n)
        {
            _num = n;
        }
            // sets the day off hours to 0
        void day_off(int day)
        {
            this->_hours[day-1] =0;

        }

        Week (const Week &s ) // consturcutor
        {
            this->_num = s._num;
            this->_hours = new double[5];


            for(int i =0; i < 5; i++)
            {
                this->_hours[i] =s._hours[i];

            }

        }
       // deconstructor
        ~Week()
        {
            delete [] _hours;
        }

        // makes the line coutable 
        friend ostream & operator << ( ostream & show, Week & wk)
        {  show << "Week: " << wk._num << " ";
           
            for (int i = 0; i < 5; i++)
            {

              show <<  wk._hours[i] << " "; 
                
             
            }
              
            return show;

        }
};

int main ()
{
    Week wk1(5);
    wk1.set_num(1);
    wk1.fill(8);

    Week wk2 = wk1;
    wk2.set_num(2);
    Week wk3 = wk1;
    wk3.set_num(3);
    Week wk4 = wk1;
    wk4.set_num(4);

    wk2.day_off(2);     //tuesday off
    wk3.day_off(5); //friday off
    wk4.day_off(1); //monday off

    cout << wk1 << endl << wk2 << endl  << wk3 << endl << wk4 << endl;

    return 0;


}

