//erik soto
//cmp223
//lab 10

#include <chrono>
#include <cstdlib>
#include <iostream>

using namespace std;

int fibonacci(int n)
{
    if (n == 0)
    {
        return 0;
    }
    else if (n == 1)
    {
        return 1;
    }
    else
    {
        return fibonacci(n-1) + fibonacci(n-2);
    }
}

int main()
{
    int n;
    int elapsed;
    int sizes[9]= {1,2,3,4,5,6,7,8,9};

    while(counter < 9)
    {
        cout << fixed << setprecision(0) << endl;
        cout << "counter: " << counter << endl;
        chrono::time_point<chrono::system_clock>start,end;
        start = chrono::system_clocl::now();

        for (int i = 1; i <= sizes[(int)counter]; ++i)
        {
            cout << fibonacci(i) << " " ;
        }
        cout << endl;

        // makes end equal end minus start
        end = chrono::system_clock::now();
        chrono::duration<double> elapsed=end-start;

        // shows the speed
        cout << fixed<< setprecision(9) << endl;
        cout << "time : " << elapsed.count() << endl;
        cout << endl;
        counter++;
    }
    return 0;
}
