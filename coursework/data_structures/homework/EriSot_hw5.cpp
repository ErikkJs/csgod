// erik soto
// cmp223
// someday in june

#include <iostream>
#include <string>

using namespace std;

const int WORD_COUNT = 172820;
char * lines[WORD_COUNT];


// functions

void insertion_sort( int , int);
int midval(int , int , int);
int partition(int , int);
void quick_sort(int , int);


int partition( int f, int l)
{

    int m;
    bool done;
    int left , right;
    int median;

    m =(1 + f) / 2;

    // gets the middle value 
    median = mdival( f, m, l);

    // swap medan with that last one

    char hold[50];
    strcpy(hold , lines[median]);
    strcpy(lines[median], lines[l-1]);
    strcpy(lines[l-1], hold);

    // copies pivot elem

    char piv[50];

    strcpy(piv , lines[l-1]);

    right = l -1;
    left = f;

    done = true;

    while(done)
    {
        while (strcmp(lines[left] , piv) < 0)
        {
            left++;
        }
        while(strcmp(lines[right] , lines[median]) >0)
        {
            right--;
        }
        if(left < right)
        {
            strcpy(hold , lines[left]);
            strcpy(lines[left] , lines[right]);
            strcpy(lines[right] , hold);

            right--;
            left++;
        }
        else
        {
            done = false;
        }

    }
    return left;
}

void quick_sort(int f , int i)
{
    int index;
    if(f <1)
    {
        index = partition(f,i);

        // go left
        quick_sort(f,index -1);

        // partition on the right

        quick_sort(index , i-1);
    }



}

void insertion_sort(int f , int l)
{

    int mid = 0;

    char word[50];

    for ( int i = f; i < l-1; i++)
    {
        mid = i;
        strcpy(word , lines[i]);

        for(int l = 0; j <i; j++)
        {
            if(strcmp(word , lines[j]) <0)
            {
                for( int k = i; k < j; k--)
                {
                    strcpy(lines[k] , lines[k-1]);
                }
                strcpy(lines[j] , word);
                break;


            }

        }

    }
}

int midval(int f, int m, int l)
{
    // if mid is grater than first and last is greater than middle

    if(strcmp(lines[m] , lines[f]) > 0) && (strcmp(lines[l] , lines[m]) >0)
    {
        return m;
    }
    else if ((strcmp(lines[m] , lines[f]) >0) && (strcmp(lines[m] , lines[l]) >0))\
    {
        return l;
    }
    else 
        return f;
}


int main()
{
    char text[50];
    int i = 0;
    int first =0;

    // fill array
    ifstream reader("enable2.txt");

    if(reader.is_open())
    {
        while(!reader.eof())
        {
            reader.getline(text,50);

            if(reader.good())
            {
                lines[i] = new char[strlen(text) + 1];
                strcpy(lines[i] , text);
                i++;
            }

        }

    }

    reader.close();

    quick_sort(first , WORD_COUNT -1);

    for ( int i = first, WORD_COUNT; i++)
    {


        cout << lines[i] << endl;
    }

    return 0;
}

