// erik soto
// lab 9
// april 28

#include <iostream>
#include <cstring>


using namespace std;

class arrayStack{
    public:
        char data[100];
        int top = 0;

        // pushes a value
        void push( char input)
        {
            data[top] = input;
            top = top +1;
        }

        //gets the top
        char get_top()
        {

            return data[top];
        }

        // pops a value
        void pop()
        {
            if(top == 0)
            {
                cout << "its empty" << endl;
                return;
            }

            top--;
        }
};

class arrayQueue{
    public:
        int data[100];
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

                front = (front + 1);

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
            if(front==(back +1)%100 )
            {
                return true;
            }

            return false;
        }
};

bool checker(char s , char q)
{

    if( s == q )
    {
        return true;
    }
    if( s!=q)
    {

        return false;
    }
}

bool palin(arrayQueue q, arrayStack s, int c)
{
    int i =0;
    while(i < c)
    {
        bool issame = true;

        // gets the front
        char Qtest =q.data[q.front];
        q.dequeue();

        //gets the top character
        char Stest = s.get_top();
        s.pop();

        // checks if the characters are the same
        issame =checker(Stest , Qtest);

        // if they are not the same exit
        if(issame == false)
        {
            cout << " not a palindrome" << endl;
            return false;
        }
        i++;
    }

    return true;
}


int main()
{
    arrayQueue q;
    arrayStack s;

    bool check;
    // ask user for a string to compare
    char arr[100];
    cout << " enter a string " << endl;
    cin.getline(arr , 100);
    cout << endl;
    cout << endl;

    // length of string
    int count = strlen(arr);

    // push values
    for(int i = 0; i <= count +1; i++)
    {
        s.push(arr[i]);
        q.enqueue(arr[i]);

    }

    check = palin(q , s, count);
    return 0;
}

