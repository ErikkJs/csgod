#include <iostream>

using namespace std;


class CartItem {
public:
    string desc;
    double price;
};

int main()
{
    CartItem * ci = new CartItem();
        
    // compile with -std=c++11
    unique ptr<CartItem> ci2 ( new CartItem);
    ci2->desc = "DVD";
    cout << ci2 -> << endl;

    delete ci;

    return 0;

}

