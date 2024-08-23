//erik soto
//feb 2 2016
//midterm prep

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

class die{

    public: 
        int nums [6];
        int roll();

        int one =0;
        int two =0;
        int three =0;
        int four = 0;
        int five =0;
        int six =0;


};
int die::roll()
{
    int random;
    // srand(time(0));

    random = rand()%6+1;

  /*switch(random)
    {
        case 1:
            {
                one++;
                break;
            }
        case 2: 
            {
                two++;
                break;
            }
        case 3:
            {
                three++;
                break;
            }
        case 4:
            {
                four++;
                break;
            }
        case 5:
            {
                five++;
                break;
            }
        case 6:
            {
                six ++;
                break;
            }

    };*/
return random;
}

int main ()
{
    srand(time(0));
    die diee;
    die diee2; 
  int num =0;
   int  roll1;
   int roll2;
    for (int i=0; i<1000000; i++)
    {
       roll1= diee.roll();
       roll2 = diee2.roll();

       
        
            if( roll1== 1 && roll2==1)
            {
                num++;
            }
        }


    
    cout << "snake eyes: "<< num  << endl;
  //  cout << diee.one << endl << diee2.one;
    //   cout << "one rolled: " << diee.one << endl <<"two rolled : " << diee.two << endl <<"three rolled : "<< diee.three << endl <<"four rolled: " << diee.four << endl << "five rolled: " << diee.five << endl << "six rolled: " << diee.six << endl;
    //roll x times
    //show the occurences 



    return 0;

};
