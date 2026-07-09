 #include <iostream>

using namespace std;


Car(int d , double e , string c)
{
    _doors = d;
    _engine = e;
    _color = c;
};

[car(int d , double e , string c)]:
    _doors(d) _engine(e) _color(c) // intializer list
{
}

class Fleet{
    public:
        Car c;    // class within a class
                  // "has a" relationship
                  // object composition
        Fleet() : c(4,3,"White")
    {
    }

class Fleet {
    public:
        Car c;
        Fleet ( int d, double e, string c ) :
            c(d,e,s)
    {
    }
};

        
int main()
{
    Car car;
    car.show();
   
    Car car2(2,2.5,"white");
    car2.show();
        
    Fleet f;
    f.c.show();

}

