// erik soto
// april 18 2016
// lab 6 
// cmp223

#include <iostream>

using namespace std;

class StackArray{
    public:
        int data[100];
        int count = 0;

        bool push(int lol) // returns false if full
        {
            if(count < 100)
            {

                data[count] = lol;
                count = count +1;
            }
            else return false;
        }


        bool pop() // returns false if empty
        {

            if(count > 0)
            {
                cout << data[count -1] << " was popped" << endl;
                count = count -1;
                return true;
            }
            else 
                cout << " its empty lol " << endl; 
            return false;
        }

        void show() // shows the values 
        {
            if( count !=0)
            { int i =count;

            for(int i = count; i >0; i--)
            {
                cout <<data[i -1] << endl;

            }

            }
            else 
                cout << " aint non in there lol" << endl;
        }
         
        void show_when_you_dump() // shows it like a tree
         {
             for( int i = count; i >0 ; i--)
             {
                 cout << data[i-1];
             }
             cout << endl;
         }

        void clear_and_dump()// clears and takes a dump
        {
            do{
               show_when_you_dump();
            }
            while(pop());


        }

};
 // the young menu
char menu()
{
    char ch;
    cout << endl;
    cout << endl;
    cout << "A - Push a value onto the stack " << endl;
    cout << "B - Pop a value from the stack " << endl;
    cout << "C - Display the contents of the stack from the top down " << endl;
    cout << "D - Clear all and display process " << endl;
    cout << "Q - Quit" << endl;
    cout << "Enter choice " << endl;
    cin >> ch;


    return ch;
}

int main()
{
    StackArray yeet;
    char choice;

    while(choice != 'Q') // as long as the user doesnt quit
    {
        choice = menu();

        switch(choice)
        {
            case 'A': // PUSHES 
                {  int num;
                    cout <<" Push what number" << endl;
                    cin >> num; 
                    yeet.push(num);

                    break;}
            case 'B':// POPS 
                {
                    yeet.pop();

                    break;
                }
            case 'C':// shows 
                yeet.show();

                break;
            case 'D':// clears and takes the int num 2
                yeet.clear_and_dump();
                break;

        }

    }

    return 0;
}

