#include <iostream>

using namespace std;


class Animal{
    private:
        string _name;
        int _eyes;

    public:
        Animal(int);
        Animal(string , int);

};
Animal ::show()
{
    cout << (_name 
    
}

Animal::Animal(int eyes)
{
    _eyes = eyes;

}
Animal animals(string name , int eyes)
{

    _name = name;
    _eyes = eyes;
    }

int main ()
{
    Animal animals[4];

    for (int i =0; i < 4; i++)
        animals[i].show();

    return 0;




}
