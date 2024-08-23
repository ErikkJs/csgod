#include <iostream>
#include <fstream>

using namespace std ;


class TennisPlayer{
    private:
        string _name;
    public:
}

class TennisMatch{
    public:
        TennisPlayer p1;
        TennisPlayer p2;

        TennisMatch(string name1, string name2)
        {
            p1.set_name(name1);
            p2.set_name(name2);

        }

        friend ostream & operator << ( ostream &out, TennisMatch &tp)
        {
            out << tp.p1.get_name() << "vs" << tp.p2.get_name();
            return out;
        }

}

int main ()

{
    TennisMatch match("Roger Federer", "novak Djokovic");
    cout << match << endl;

    ofstream fs;
 
    fs.open("tennis.txt");
 
    fs << match << endl;
 
    fs.close();
 
    return 0;




}

