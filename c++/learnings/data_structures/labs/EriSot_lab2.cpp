//  Purpose: This code will intentionally crash so we can use gdb.
//  This crashes because the array is not allocated before being accessed.

#include <iostream>
#include <stdio.h>
using namespace std;

void printArray(int *, int);

int main()
{
 int size;
  
    
  cout << "Enter size of array: ";
  cin >> size;

  int *a = new int [size];
  
  cout << "Enter " << size << " elements: ";
  for(int i = 0; i < size; i++)
  {
    cin >> a[i];
  }

  cout << "You entered:";
  printArray(a, size);
  cout << endl;

  return 0;
}

void printArray(int *arr, int size)
{
  for(int i = 0; i < size; i++)
  {
    cout << " " << arr[i];
  }
}
