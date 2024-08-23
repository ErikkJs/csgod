#include <iostream>

using namespace std;



class Rect{
    private:
        double _width;
        double _height;

    public:
        rect( double w, double h)
        {
            _width = w;
            _height = h;
        }

        double area()
        {
            return _width * _height;
        }

        int compare (Rect r)
        {
            double a = this.area();
            double b = r.area();
            if ( a < b)
                return -1;
            else if (a==b)
                return 0;
            else  (a > b)
                return 1;
        }

        friend void show(const Rect &r);
};
void show(const rect &r)
{
    cout << "Width: " << r. _width << endl;
    cout << "Height: " << r._height << endl;
    cout << "Area: " << r._area() << endl;

}



int main ()
{
    Rect r1(10 , 20);
    Rect r2(10 , 10);

    show (r1);

    int comparison = r1.compare(r2);
    if (comparison == -1)
        cout << "less\n";
    else if (comparison ==0)
        cout << "equal\n";
    else if(comparison ==1)

    else
    cout << " N/A"; << endl;

    cout << "Area: " << r1.area() << endl;

    return 0;


}




























