#include <iostream> 

using namespace std;


class Profile{

    public:
        int age;
        int friends;

        friend bool operator <(const Profile &p1, const Profile &p2)
        {
            return p1.friends < p2.friends;
        }


        bool operator<(const profile &p2)
        {
            return this->friends < p2.friends && (this->age < p2.age);
        }

        profile operator+(int value)
        {
            profile.temp;
            temp.friends = this ->friends +value;
            return temp;
        }

}



int main()
{

    Profile p1,p3;

    p1.friends = 100;
    p1.age =13;
    p3.friends = 200;
    p3.age = 65;

    if( p1.friends < p2.friends &&p1.age < p3.age)
    {
        cout << "less" << endl;
    }


}

