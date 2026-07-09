#include <iostream>

using namespace std;

class ArrayInt{
    private:
        int * _array;
        int _size;
    public:
        ArrayInt(int s)
        {
            _size = s;
            _array = new int[s];
        }

        ~Array()
        {

            delete _array;
        }
};

template <class T>
class Array{
    private:
        T * _array;
        int _size;
    public:
        Array(int s) 
        {
            _size = s;
            _array = new T
        }
        T & operator[] ( int index)
        {
            if(index <0 || index >= _size)
                throw 1;
            return _array[index];
        }

        int get_length()
        {
            return size;
        }


};


int main()
{
    Array < double > ad(10);

    for ( int i = 0; i < ai.get_length(); i++)
    {
        ai[i] = i + .01;
    }

    for ( int i = 0; i < ai.get_length(); i++)
    {




}

