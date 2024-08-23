#include <iostream>
#include <fstream>
#include "GameLevel.h"

using namespace std;

int main()
{
   GameLevel level1;
    level1.load ("level.txt");
    level1.set_gap(3);

    GameLevel level2 = level1;
    level2.set_gap(2);

    level2.adjust_heights(-1);

    cout << "Level 1" << endl;
    cout << level1;

    cout << "Level 2" << endl;
    cout << level2;

    return 0;

}
