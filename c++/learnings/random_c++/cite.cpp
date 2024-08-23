#include <iostream> 

using namespace std;


class CiteGen{
    private:
        string fir;
        string pub;
        string las;
        string tle;
        string cit;
        int yer;
        bool medium;
    public:
        CiteGen(string f , string l , string p , string t, string c , int y , bool m)
        {
            fir = f;
            pub = p; 
            las = l;
            tle = t;
            cit = c;
            yer = y;
            medium = m;
        }
        void ShowCite()
        {
            cout << las << ", " << fir << ". " << tle << ". " << cit << ": " << pub << ", " << yer << ". "; 
            if( medium == true)
            {
                cout << "Web.";
            }
            else 
                cout << "Print.";
        }

};

int main ()
{
    string first;
    string last;
    string publisher;
    string title;
    string city;
    int year;
    char pick;
    bool medium =  false; 

    cout << " Enter the first name " << endl;
    cin >> first;

    cout << " Enter the last name " << endl;
    cin>> last;

    cout << " Enter the title " << endl;
    cin >> title;

    cout << " Enter the publisher name " << endl;
    cin >> publisher;
   
    cout << " Enter the  place of publication" << endl;
    cin >> city;

    cout << " Enter the year " << endl;
    cin >> year;
   
    cout << " is this print? " << endl;
    cout << " (Y)es , (N)o" << endl;
    cin >> pick;

    if ( pick == 'N' )
    { 
        medium == true;
    }
    CiteGen Yeet(first, last, publisher, title, city, year, medium); 

    Yeet.ShowCite();

    return 0;

}

