/*   static arrays versus dynamic arrays      */

#include "cmpslib.h"
struct LOGGER_DATA LGDATA; // create a global instance of this struct .. needed by logging library




void PrintArray(int*array,int size)
  {
  LogStart(array,size);
  cout << "Array Address" << array << endl;
  for(int loop =0; loop<size; loop++)
    {
    if(loop)
      {
      cout << ",";
      }
    cout << array[loop];
    }
  cout << endl;
  LogEnd(array,size);
  }

int main()
  {
  LoggerSetUpLog("main1.log");

  // create a static int array of size 20 named SIntArray
  int SIntArray[20];

// create a dynamic int array of size 20 named DIntArray
   const int SIZE = 20;
   int *DIntArray;
   DIntArray= new int[SIZE];
// use a for loop to populate both arrays with the values 0-20 , use the [] operator

   for(int i=0; i <20; i++)
   {
       SIntArray[i]=i;
       DIntArray[i]=i;
   }

// use the PrintArray functions to print them both out

   PrintArray(SIntArray,20);
   PrintArray(DIntArray,20);
// use a for loop to populate both arrays with the values 0-20, do not use the [] operator

   for(int i=0; i<20; i++)
   {
       int *ptr;
       ptr=&DIntArray[i];
       *ptr=i;
       ptr=&SIntArray[i];
       *ptr=i;

   }

// use the PrintArray functions to print them both out

   PrintArray(DIntArray,20);
   PrintArray(SIntArray,20);


// print out the address of the first  , second and fifth element of both arrays, use the & and [] operators

// print out the address of the first  , second and fifth element of both arrays, do NOT  use the & and [] operators

// create an int * temp

// set temp to the begining of SIntArray

// increment the pointer by 4

//  print out the value at that position via temp

// set temp to the begining of DIntArray

// increment the pointer by 4

//  print out the value at that position via temp

// clean up the Dynamic array

  return 0;
  }






