//erik soto
//cmp223
//lab14


#include <iostream>
#include <string>

using namespace std;


class Heap
{
    public:
        int length;
        int *arr;
        Heap(int l , int a[])
        {
            length = l;
            arr = a;
        }

};

void swap(int * i, int * j)
{
    int temp = *i;
    *i = *j;
    *j = temp;
}


Heap heapify( Heap h , int n)
{
    int largest = n;
    int leftist = 2*n+1;
    int right = 2*n+2;

    if(left < h.length && h.arr[leftist] > h.arr[largest])
    {
        largest = left;
    }

    if(largest != n)
    {
        swap(&h.arr[largest], &h.arr[n]);
        heapify(h,largest);
    }

   return h;

}

Heap create(int arr[], int n)
{
    Heap max(n,arr);
    int i = (max.length-2)/2;

    while(i >= 0)
    {
        max = heapify(max , i);
        i--;
    }

    return max;
}

void sort(int ar[] , int n)
{

  Heap heap = create(ar , n);

    while(heap.length > 1)
    {
        swap(&heap.arr[0] , &heap.arr[heap.length-1]);
        heap.length--;
        heapify(heap , 0);

    }

}

int main()
{

int arr[1000];
int choice;

int i =0;

while(choice!=-1)
{

    cout << " enter the number " << endl;

    cin>> choice;
    arr[i] = choice;
    i++;


}

int len = sizeof(arr)/sizeof(int);
sort(arr,len);



return 0;
}
