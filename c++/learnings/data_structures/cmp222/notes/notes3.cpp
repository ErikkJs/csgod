#include <iostream>

using namespace std;



class Animal{
    private:
        int _eyes;

    public:
        static int count;
        Animal(int eyes)
        {
            _eyes = eyes;
            count ++;

        }
        static int get_count;
        {

                return count;

        }

};

int Animal::count = 0;




int main()
{

    Animal a1(2);

    cout <<Animal::count << endl; 

        Animal a2(2);

    cout << Animal::count << endl; 

        Animal a3(2);

    cout << Animal::count << endl;


return 0;

}

