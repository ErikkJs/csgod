// erik soto
// lab 12
// cmp223

#include <iostream>
#include <fstream>
using namespace std;

//hash + w -'a'+1

int main()
{
    int collisions[320];

    for(int i =0; i< 320; i++)
    {
        collisions[i] = 0;
    }

  
    return 0;
}

int rudimentaryhashfunction(int yeet[], string line)
{

    string line;

    ifstream ifs("enable.txt");
    if( ifs.is_open())
    {
         while(!ifs.eof())
         {
             ifs >> line;

             if(ifs.good())
             {
                 int hash=0;
                  char w;
                 for(int i =0;i < line.length(); i++)
                 {
                     w = word[i];
                     hash = hash + w - 'a' + 1;

                 }
                 yeet[hash] = yeet[hash] +1;

             }
         }
    }
ifs.close();
}

