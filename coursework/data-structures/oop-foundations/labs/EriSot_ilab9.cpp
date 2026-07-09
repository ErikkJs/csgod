#include <iostream>

using namespace std;
#include <cctype>

const int NO_LOWER =1;
const int NO_UPPER =2;
const int NO_DIGIT =4;
const int TOO_SHORT =8;
const int TOO_LONG = 16;

int validate_password(string p)
{
    int error =0;
    int lower =0;
    int upper =0;
    int nums =0;
    int length =0;
    int Llength =0;

   // checks for lowers
    for(int i =0; i < p.length(); i++)
    {

        if ( islower(p[i]) )
        {
            is_lower++;
        }
    }
    
    // checks for uppers 
    for (int i =0 ; i < p.length(); i++)
    {
        if ( isupper(p[i]))
        {
            is_upper++;
        }
    }

    // checks for numbers
    for(int i =0; i < p.length(); i++)
    {
        if(isdigit(p[i]); i++)
        {
            is_nums++;
        }
    }

    for(int i =0; i < p.length(); i++)
    {
        i = length;
    }
    
    for(int i =0; i < p.length(); i++)
    {
        i = Llength;
    }

    if(lower < 1)
        error = error | NO_LOWER;

    if(upper < 1)
        error = error | NO_UPPER;

    if (nums <1)
        error = error | NO_DIGIT;

    if(length <8)
        error = error | TOO_SHORT;
  
    if(length >16)
        error = error | TOO_LONG;

    return error;
}

int main ()
{
    string pass;
    cout << " Enter a password" << endl;
    cin >> password;
    // ask for a string 
     
    // validate that the string has : one lower , one upper , one digit
    
    // also validate that the length of string is 
    // at least one lower one uppper one digit
    
return 0;
}

