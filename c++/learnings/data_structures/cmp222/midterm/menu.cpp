#include <iostream>

using namespace std;



int main ()
{
    Menu menu;
    menu.add('a', "Add two numbers");
    menu.add('s', "Subtract two numbers");

    menu.allow_upper = true;
    menu.add('q', "Quit");

    while(menu.show())
    {
        switch(menu.choice)
        {


        }

    }
    return 0;

}

