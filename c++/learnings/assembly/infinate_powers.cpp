#include <iostream>

using namespace std;

double get_num();
double get_solution(double num );

int main()
{
    double num,power;


    cout << " enter a number" << endl;
    num = get_num();

    cout << "that number squared equals: " << get_solution(num)<< endl;

    return 0;
}



    double get_solution (double num)
    {
        double return_value;

        return_value =   num * num;

        return return_value;
    }

    double get_num()
    {
        double num;

        cin >> num;

        while (num < 1)
        {
            cout << "cant be less than 1" << endl;
            cin >> num;
    
        }
    return num;
    }






