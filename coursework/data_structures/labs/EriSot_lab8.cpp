// erik soto
// cmp 223 lab 8
// april 26

#include <iostream>
#define capacity = 10;
using namespace std;

class arrayQueue{
    public:
        int data[10];
        int front =0;
        int back = 0;

        // adds value to array
        bool enqueue(int val)
        { 
            data[back] = val;
            back = (back +1)%10;

        }

        // adds value when full
        int dequeue()
        {
            bool check = is_empty();
            if(check == true )
            {

                return false;
            }

            else
            {

                front = (front + 1) %10;

            }

        }

        // checks if the array is empty
        bool is_empty()
        {

            if(back == front)
            {
                cout << " is empty" << endl;
                return true;
            }
            return false;

        }

        // checks if full
        bool is_full()
        {
            if(front==(back +1)%10 )
            {
                return true;
            }
            return false;

        }
        // shows the values in the array 
        void show()
        {
            int i=front;
            while( i!=back)
            {
                cout << data[i] << " " << endl;
                i =(i+1)%10;
            }

        }

};


int main()
{
    arrayQueue yeet;

    int i =1;
    do {
        int num;
        bool check;

        // ask user for number and assaign
        cout << " enter a number " << endl;
        cin >> num;
        cout << endl;

        // if  the array is full
        if(yeet.is_full())
        {
            yeet.dequeue();
            yeet.enqueue(num);
        }
        else 
            // adds to array
            yeet.enqueue(num);

        // shows array
        yeet.show();



    }
    while(i != 100);

    return 0;

}

