#include <iostream>

using namespace std;

struct Student{
    string name;
    int age;
};

Student *create_student(string n , int a)
{
    Student  *Allah = new Student;

    Allah->name = n;
    Allah->age = a;


    return Allah;


}


int main ()
{

    Student *s = create_student("Allah" , 89);

    cout << s->name << " is " << s->age << endl;

    delete s;

    return 0;
}

