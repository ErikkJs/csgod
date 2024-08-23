#include <iostream>

using namespace std;



class Party{
    public:
        string voted;
        virtual void vote() = 0;
        string  get_vote()
        {
            return voted;
        }

};

class Democrat : public Party {
    public:
        void vote()
        {
            voted = "Democrat";

        }

        Democrat()
        {

            vote();

        }
};

class Republican : public Party{
    public:
        void vote()
        {
            voted = "Republican";

        }

        Republican()
        {

            vote();

        }
};

class Independant : public Party{
    public:
        void vote()
        {
            voted = "Independent";

        }

        Independant()
        {

            vote();

        }
};

void show_vote(Democrat d)
{
    cout << " voted " <<  d.get_vote()<< endl;
}

void show_vote(Republican r)
{
    cout << " voted " << r.get_vote() << endl;
}
void show_vote(Independant i)
{
    cout << " voted " << i.get_vote() << endl;
}


int main ()
{
    Democrat d; 
    Republican r;
    Independant i;

    show_vote(d);
    show_vote(r);
    show_vote(i);


    return 0;
}

