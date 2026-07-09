//Erik Soto
//cmp 223
//lab 11

#include <iostream>
#include <chrono>
#include <cstdlib>
#include <iomanip>
using namespace std;

// binary search
int binary_search( int * data, int front , int back , int value)
{
    int mid;
    int found;
    mid = (front + back)/2;

    // returns value if they match
    if(data[mid] == value )
    {
        return value;
    }
    // if it never equaled
    else if( front == back)
    {
        return -1;
    }

    //checks if its less than mid
    else if(value < data[mid])
    {
        found = binary_search(data,front , mid -1, value); 

    }

    // checks if its more than mid
    else if(value > data[mid])
    {
        found =binary_search(data, mid + 1 , back  , value); 
    }

}


int main()
{
    int count;
    int yeet;
    int front;
    int back;
    int res;
    int array[15] = {7, 11 ,21 , 25 , 27 , 30 , 32 , 49 , 50, 51 , 60 , 80, 201, 300 , 301};
    // new int =arr[100000000];


    cout << " enter a number " << endl;
    cin >> yeet;

    front=0;
    back= 14;

    res= binary_search(array,front, back,yeet);

    if( res == -1)
    {
        cout << " NOT FOUND " << endl;
    }
    else
    {
        cout << " the number " << yeet << " was found " << endl;
    }



    // _______________________________SECOND PART LOL ____________________________


    int *arr;
    arr= new int[100000000];
    for(int i= 0; i < 100000000; i++)
    {
        arr[i] =rand()%5+1;

    }
    count = 0;

    cout << " enter num " << endl;
    cin >> yeet;

    chrono::time_point<chrono::system_clock>start,end;
    start =chrono::system_clock::now();

    // looks for the number entred 
    int f = binary_search(arr, 0 ,99999999, yeet);
   
    // checks if number was found
    if( yeet==arr[f])
    {
        cout << " found" << endl;
        count++;
    }
    else 
    {
        cout << " NOT FOUND" <<endl;
        count++;
    }
    // times how long it took 
    end = chrono::system_clock::now();

    chrono::duration<double> elapsed = end-start;
    
    
    cout << fixed <<  setprecision(10) << " found in " << elapsed.count() <<" seconds" << endl;
    cout << " took " << count << " steps to complete" << endl;

    delete arr;
    return 0;
}

