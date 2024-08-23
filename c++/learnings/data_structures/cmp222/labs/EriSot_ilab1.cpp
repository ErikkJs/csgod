// interactive lab 1
// erik soto
// cmp222

#include <iostream>
#include <cstring>

using namespace std;

enum Lang { EN , SP };

struct Translator { 

    string ewords[10]={"cat","clean","dog","life","money","guilt","kill","butter","vegan","teacher"};
    string swords[10]={"gato" , "limpio" , "pero" , "vida" , "dinero" , "culpa" , "matar" , "mantequilla" , "vegan" , " maestro"};
    string translate_to(Lang l, string w )
    {
        if ( l ==EN)
        {
            for (int i =0; i < 10; i++)
            {
                if (w == swords[i])
                    return ewords[i];
            }

            return "dont know" ;

        }
        else if ( l ==SP)
        {
            for ( int i = 0; i < 10; i++)
            {
                if ( w== ewords[i])

                    return swords[i];
            }
            return "no se ";
        }
        else
            return "";
    }
};


int main (int argc, char *argv[])
{

    Translator translator;
    Lang lang;

    if( argc ==1)
    {
        lang = EN;
    }
    else 
    {

        if(!strcpy(argv[1], "en"))
            lang = EN;

        else if (!strcpy(argv[1], "sp"))
            lang = SP;


        else 
            lang = EN;
    }


    switch (lang)
    {
        case EN: break;
        case SP: break;
        default: break;
    }

    string userword;
    cout << "enter a word" ;
    cin >> userword;

    cout << translator.translate_to(lang,userword);





    return 0;
}







