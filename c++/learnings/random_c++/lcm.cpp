#include <iostream>

using namespace std;

int LCM (int x, int y);
int main(){

    int a,b,c;
    cin >> a;

    cin >>b;

    LCM(a, b);

    return 0;

}
int LCM(int x, int y)
{
    bool flag = true;
    int result;
   if (x > y)
   {
       result = x;
   }
   else { result = y; }
    do{
        if((result%x == 0) && ( result%y  ==0)){
        cout << result;
        return result;
        flag = false;
        }
        result++;
    }
    while(flag);
}
