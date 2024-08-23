//erik soto
//cmp223
//hw3
//april 22

#include <string>
#include <cstring>
#include <iostream>
#include <cstdlib>

using namespace std;

class arrayStack{
    public:
        double data[100];
        int count;
        //------------Constuctor---------------- \\

        arrayStack()
        {
            count = 0;
            for(int i = 0; i <100; i++)
            {
                data[i]=0;
            }


        }
        //------------Checks if empty------------\\

        bool empty()
        {
            if(count == 0)
            {
                return true;
            }
            else 
                return false;
        }
        //-----------Checks if Full--------------\\

        bool full()
        {
            if(count == 100)
            {
                return true;
            }
            else 
                return false;
        }
        //-----------Pushes a value--------------\\

        bool push(int num)
        {
            bool check =full();
            if(check == true)
            {
                cout << " The array is full " << endl << endl;
                return false;
            }
            else 
                data[count] = num;

            count = count +1;

            return true;

        }
        //----------Pops a value---------------\\

        bool pop()
        {
            bool check = empty();
            if(check == true)
            {
                cout << "cant do that its empty " << endl << endl;
                return false;
            }
            else

                count = count -1;
            return true;
        }
        //----------Shows top value --------------\\

        double top()
        {
            bool check = empty();
            if(check == true)
            {
                cout << " its empty " << endl << endl;
                return 0;
            }
            else
                return 
                    data[count-1];
        }
        //evaluates based on input
        void eval(int n1, int n2, char op )
        {
            switch(op)
            {
                case '+':
                    push(n1 + n2);
                    break;

                case'-':
                    push(n1 - n2);
                    break;

                case '*':
                    push(n1 * n2);
                    break;

                case '/':
                    push(n1/n2);
                    break;

            }

        }
};
bool isInteger(char *text)
{
    for(int i =0; i < strlen(text); i++)
    {
        if( i ==0 && (text[i] == '-' || text[i] == '+'))
        {
            if(strlen(text) ==1)
            {
                return false; // length of 1 means operator
            }

            else if (!isdigit(text[i]))
            {
                return false;
            }
        }
    }
    return true;
}

bool isDouble(char *text)
{
    int dots =0;
    for(int i =0; i < strlen(text); i++)
    {
        if(text[i] == '.')
        {
            dots++;
            if(dots>1)
            {
                return false;
            }
        }

        else
            if (i == 0 && (text[i] == '-'||text[i] == '+'))
            {
                if(strlen(text)==1)
                    return false; // length 1 means operator
            }
            else if(!isdigit(text[i]))
                return false;
    }

return true;
}

bool isOperator(char *text)
{
    if(strlen(text) >1)
    {
        return false;
    }
    if(text[0] == '+' || text[0] == '-' || text[0] == '*' || text[0] == '/')
    {
        return true;
    }

    return false;
}

int main()
{

    bool is_empty;
    arrayStack al;
    char line[100];
    cout << " Enter the string " << endl;
    cin.getline(line,99);

    int count = 0;
    //iterates count to see how things there is 
    for(int i =0; i< strlen(line);i++)
    {
        if(isspace(line[i]))
        {
            count++;
            while(isspace(line[++i]));
            i--;
        }
    }

    // makes tokens
    char **tokens;
    try{

        tokens = new char*[count+1]; // number of tokens
    }
    catch(bad_alloc)
    {
        tokens = NULL;
    }

    // assigns to the tokens
    int slot = 0;
    int len = strlen(line);
    int i =0;
    while (i < len)
    {
        if(!isspace(line[i]))
        {
            tokens[slot] = &(line[i]); // gets the address of the token
            slot++;
            while(!isspace(line[++i]) && i<len)
            {
                tokens[slot]=&(line[i]);
            }
        }
        else
        {
            line[i++] = '\0';
        }

    }
    for(int i =0; i<count+1; i++)
    {
        if(isInteger(tokens[i]))
        {
            int num = atoi(tokens[i]);
            // push the number onto the stack
            al.push(num);        
        }
        else if (isDouble(tokens[i])==true)
        {
            double num =atof(tokens[i])==true;
            al.push(num); 
        }
        else if(isOperator(tokens[i])==true)
        {
            char op = tokens[i][0];
            bool check;
            double num1 = al.top();
            check = al.pop();
            if(check == false)
            {
                cout << " invalid " << endl;
                break;
            }


            double num2 = al.top();
            check =  al.pop();
            if(check ==false)
            {
                cout << "invalid" << endl;
                break;
            }
            // evaluates 
            al.eval(num1, num2, op);

        }

        else 
        {

            cout << "badly formed expression" << endl;
            break;;
        }
    }
    // checks to give answer
    is_empty= al.empty();
    if(is_empty == false)
    {
        bool check;
        double topone;
        topone = al.top();
        check = al.pop();
        if(check == true)
        {
            cout << topone << endl;
        }
        else 
            cout << " invalid expresion " << endl;
    }

    delete tokens;
    return 0;
}

