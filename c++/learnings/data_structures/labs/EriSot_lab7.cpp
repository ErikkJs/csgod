// erik soto
// lab 7
// Hitlers birthday

#include <iostream>
#include <cstring>
#include <string>

using namespace std;

class Brackets{
    public:
        char stack[100];
        int yaht = 0;
         
        // pushes a value and itterates
        bool push(char goat)
        {
            if(yaht = 100)
            {
                return false;
            }

            stack[yaht] = goat;
            yaht++;

        }

        // decreases the count of yaht
        void pop()
        {
           
            yaht--;

        }

        // ccheks the emptyness
        bool is_empty()
        {
            if(yaht = 0)
            {
                cout << "its empty lol" << endl;
                return true;
            }
            else 
                return false;
        }

        // shows the value
        void show()
        {
            for(int i =yaht; i>0; i--)
            {
                cout << stack[yaht] << endl;
            }

        }
        // checks for brackets 
        bool check_brackets( char * t)
        {
            bool bal = true;
            int len = strlen(t)+1;

            int i =0;
            while( i!=len)
            {
                if(t[i] == '(')
                {
                    push('('); 
                }
                else 

                    if(t[i]== ')')
                    { 
                        pop();
                    } 
                i++;
            }
            if(yaht < 0)
            {
                bal = true;
            }
            if(yaht == 0)
            {
                bal = false;
            }


          return bal;

        }

};
int main()
{ 
    Brackets yeet;
    bool lol=true;
    string holder;

    cout << " enter a string " << endl;
    cin>>holder;

    char line[holder.size()+1];
    strcpy(line, holder.c_str());

    lol =yeet.check_brackets(line);

    if(lol == false)
    {
        cout << " READY" << endl;
    }
    else 
        cout << " INCOMPLETE " << endl;

    return 0;
}
