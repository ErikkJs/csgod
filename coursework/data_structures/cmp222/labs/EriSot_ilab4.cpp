#include <iostream>

using namespace std;



class Human{
    protected:
        int eyes;
};

class Worker : public Person{
    public:
        string occupation;

        Worker( string job )
        {
            occupation = job;
        }
};

class Patient : public Person{
    public:
        string disease;
        int limbs_cut_off;

};


class Person : public Human{
    public: 
        Person(int ojos)
        {
            eyes = ojos;
        }
        string name;
        void show()
        {
            cout << name << " has " << eyes << " eyes\n";
        }

};

int main()
{
    Person p(2);
    p.name = "Jessica Marshall";
    p.show();

    Worker w("Baker");


    return 0;

}

