// erik soto
// cmp223
// lab3
// april 7
#include <iostream> 

using namespace std;

struct ArrayList{
    int data[100];
    int count = 0;
};

// adds to the array
bool add(ArrayList &a1 , int value)
{
    if(a1.count <100)
    {
        a1.data[a1.count] = value;

        a1.count++;

        return true;
    }
    else

        return false; 

}

// exposes it self to the world 
void display( ArrayList a1 )
{
    for( int i=0; i < a1.count; i++)
    {

        cout << a1.data[i];
    }
    cout << endl;
}

// finds a value in the array to see if its there and tells you where its at
int find(ArrayList &a1, int value)
{
    int i =0;
    while(a1.data[i] != value)
    {
        i++;
    } 
    if(a1.data[i] == value)
    {
        return i;
    }
    else 
        return -1;

}
// delEATS a given value. #feedthefunctions
bool deleat(ArrayList &a1, int value)
{
    int find1;
    find1 = find(a1 , value); 

    if(find1 != -1)
    {
        for(int i =find1; i <a1.count; i++)
        { 
            a1.data[i] = a1.data[i+1];
        }
        a1.count = a1.count -1;
        return true;
    }
    return false;
}  
// emptys the array by setting count to 0
void empty(ArrayList &a1)
{
    a1.count = 0;
}
// inserts a given value at any point 
bool insert(ArrayList &a1 , int index , int value)
{

    for( int i = a1.count -1; i >= index; i--)
    {  
        a1.data[i +1] = a1.data[i];      

    }

    a1.data[index] = value;
    a1.count++;
    return true;
}


// mcdonalds menu 
char menu()
{
    char yeet;

    cout << "Add" <<" " <<  "A" << endl;
    cout << "Display" << " " << "B" << endl;
    cout << "Find" << " " << "C" << endl;
    cout << "Delete" << " " << "D" << endl;
    cout << "Clear" << " " << "E" << endl;
    cout << "Insert" << " " << "F" << endl;

    cin >> yeet;

    return yeet;
}

int main()
{   ArrayList woo;

    char djk; 

    // runs as long as your choice is not quit
    do{ 
        djk = menu();

        switch(djk)
        {
            bool ieiei;
            int found;
            
            // insert a number function 
            case 'A':
            int insertnum;
            cout << " insert what number" << endl;
            cin >> insertnum;

            ieiei= add(woo , insertnum);

            if ( ieiei == true)
            {
                cout <<" Added" << endl;
            }
            else 
                cout << " YEEEEET" <<endl;

            break;

            // display function 
            case 'B':
            display(woo);

            break;

            // find function 
            case'C':
            found = -1;
            found = find(woo, 2);
            if ( found != -1 )
            {
                cout << " the number "  << " was found at " << found <<  endl;
            }
            else 
                cout << " NOT found " << endl; 

            break;

            // delete function 
            case'D':
            ieiei=  deleat(woo , 3);
            if(ieiei == true )
            {
                cout <<  " success" << endl;
            }
            else 
                cout << " failure " << endl;
            break;
                
            // empty array call
            case'E':
            empty(woo);
            cout << " DELETED !!!! " <<endl;
            break;

            // insert function 
            case 'F':
            ieiei = insert(woo, 3 ,5);
            if( ieiei == true)
            {
                cout << " success" << endl;
            }
            else 
                cout << "failure" << endl;

            break;
        }
    }
    while( djk != 'Q');

    return 0;
}

