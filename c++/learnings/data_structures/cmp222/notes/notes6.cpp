#include <iostream>

using namespace std;

class Student{
    public:
        int id;
        int age;
        int *scores;

        //makes dynamic array

        Student(int size)
        {
            scores = new int[size]

        }

        ~Student()
        {

            delete [] scores;

        }

        Student(const Student& s, int size)
        {
            this ->id = s.id;
            this ->age = s.age;
            this ->numscores = s.numscores;
            this ->scores = new int scores[];
            for(int i =0; i < this ->numscores; i++)
            {
                this->scores[i] = s.scores[i];
            }


}

Student::Student(const Student &s, int size) 
{
    this->age = s.age;
    this->id = s.id;
    this->scores = new int [size];
}





int main ()
{

    Student s1(5);
    s1.id =10001;
    s1.age = 18;
    s1.scores[0]= 100;

    Student s2 = s1;

    cout << s1.id << " " << s1.age << " " << s1.scores[0] << endl;

    cout << s2.id << " " << s2.age << " " << s1.scores[0] << endl;


}

