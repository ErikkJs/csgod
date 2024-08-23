#include <iostream>

using namespace std;

enum TransactionType { UNKNOWN, CARRY, DEBIT, CREDIT };

struct Transaction {
    string desc;
    double amount;
    TransactionType trans;
};

class Account {
    private:
        int _count;
        double _balance;
        Transaction _items[100];

        void new_trans(double, string, TransactionType);
    public:
        void deposit(double);
        void charge(double, string);
        double get_balance();
};

void Account::Account(int amount)
{

    _balance = amount;

}


void Account::deposit(double amount)
{
    // notice how we call the private member function new_trans
    // and pass it parameters that define a Deposit


    new_trans(amount, "DEPOSIT", CREDIT);
}

void Account::charge(double amount, string desc)
{
// checks balance then compares it to the amount that is spent 
// if it costs too much it tells you you cant afford it                                
    int b = get_balance();

    if( b < amount)
    {
        cout << " you cant afford this" << endl;
    }
    else{
                                                       
            new_trans(amount, desc, DEBIT);
   
   
   
    }

}

void Account::new_trans(double amount, string desc, TransactionType trans)
{
    // fills the array _items 
    // sets the parameters asvalues as well
    
        _items[_count].amount = amount;
        _items[_count].trans = trans;
        _items[_count].desc = desc;

        if( _items [_count].trans = DEBIT)
        {
            _items[_count].amount = amount *-1;
        }
                _count ++;
    


    // used the count ++ to leave the last location of the vacant array spot



}

double Account::get_balance()
{
    double result = 0;
    double sum =0;

    // calculate all items int the array to find out the total balance.
   
    for ( int i =0; i <_count; i++)
    {   
       sum = sum;
      sum = sum + _items[i].amount;
                

    }
result= sum;


    return result;
}

char show_menu();

int main()
{
    Account checking(100);
    string desc;
    double amount;
    char entry;

    do
    {
        cout << checking.get_balance() << endl;
        entry = show_menu();
        
        switch (entry)
        {
            case 'd': // DEPOSIT
                {
                    //ask user for the amount of the deposit

                   cout << " how much do you wish to deposit? " << endl;
                       cin>> amount;

                    checking.deposit(amount);
                    break;
                }

            case 'p':   // CHARGES/PURCHASES
                {
                    // ask user for description
                    // as well as amount of the trasacation 
                    
                    cout << " enter purchases amount " << endl;
                        cin >> amount;
                    
                        cout << " enter description" << endl;
                    cin >> desc;

                    checking.charge(amount, desc);
                    break;
                }
        }

    } while (entry != 'q');

    return 0;
}

char show_menu()
{
    char c;

    do
    {
        cout << "(D)eposit, (P)urchase, (Q)uit: ";
        
        cin >> c;
    } while (c != 'd' && c != 'p' && c != 'q');

    return c;
}
