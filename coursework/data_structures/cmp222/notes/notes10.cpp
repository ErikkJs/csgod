#include <iostream>

using namespace std;

class Book {
    private:
        string _name;
    public:
        Book() {}
        Book(const Book &b) {}
};


int main ()
{  
Book book1;
Book book2 = book1;

return 0;


}

