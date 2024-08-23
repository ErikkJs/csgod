#include <iostream>
using namespace std;

char choose_animal()
{
    char c;

    cout << " (A)nimal , (D)og , (Cow) ";
    cin >> c;

    return c;
};

class Animal{
    public:
     
       /* virtual  void Speak()
        {
            cout << "Non Human Utterence\n"; 
        }
*/
virtual void Speak() = 0;  
};


class Dog : public Animal{
    public: 
        void Speak()
        {
            cout << "Woof\n";
        }

};
class Cow : public Animal {
    public:
        void Speak()
        {
            cout << "Moo\n";
        }

};

int main()
{

    Animal *a;

    switch (choose_animal())
    {
      //  case 'a' : a = new Animal(); break;
        case 'd' : a = new Dog(); break;
        case 'c': a = new Cow(); break;
    }

    a->Speak();

    delete a;

    return 0; 
}

