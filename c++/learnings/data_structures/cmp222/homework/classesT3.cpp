#include <iostream>

using namespace std;


int main()
{
    vector<int> v;
    // or can be called ]
     v.reserve(100);

    for (int i=0; i <10; i++)
    {
        v.push_back(i);
    }


    vector<int>::iterator j = v.begin();
    while(j != v.end())
    {
        cout << *j << endl;
        j++;
    }

return 0;
}

