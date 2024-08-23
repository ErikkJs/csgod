
  
#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <iomanip>
#include <stdexcept>   
#include <stdio.h>
#include "range_error.h"

using namespace std;

template <class T>
class List  {
  template <class T1>
  friend ostream & operator << (ostream &, List<T1> & );

public:
  
  List();
  void add(const T &);
  void print(int);  
  int getSize();
  void swap(int, int);
  int max(int, int);
  void sort();

  static const int MAX = 10;

private:
  T list[MAX];
  int size;
};

/* constructor */
template <class T>
List<T>::List() {
  size = 0;
}


/* add element to list */
template <class T>
void List<T>::add(const T &value) {
  // surround the out-of-bounds check in a try block
  try {
    if (size >= MAX)
        throw RangeError (__FILE__, __FUNCTION__, __LINE__, size);
  }
  // catch block for RangError
  catch (const RangeError &err) {
     throw err;  /* rethrow exception back to calling function */
  }
  // if we made it here we can safely add the value
  list[size++] = value;
}


template <class T>
void List<T>::print(int length) {
  // format for doubles just in case 
  cout << setiosflags( ios::fixed | ios::showpoint ) << setprecision( 1 );

  /* demonstrate out_of_range, a pre-defined exception from C++ stdexception 
   * library */ 
  try {
     if (length > size)
        throw out_of_range ("print: out_of_range");
  }

  // every exception in the standard lib has a what() method
  catch (const out_of_range err) {
      cerr << err.what() << endl;
      throw err;
  }

  // if we made it here we can safely display the list 
  for (int i = 0; i < length; i++) {
     cout << setw(7) << list[i];
  }
  cout << endl;
}

template <class T>
int List<T>::getSize() {
  return size;
}


// swap the value at position i with the value at position j 
template <class T>
void List<T>::swap(int i, int j) {

  try {
    if (i < 0 || i >= size || j < 0 || j >= size) {
      throw RangeError (__FILE__, __FUNCTION__, __LINE__, j);
    }
  }
  catch (const RangeError &err) {
    cerr << err;
    throw err;
  }

  T temp;
  temp = list[i];
  list[i] = list[j];
  list[j] = temp; 
}

/* return the larger position of the ith and jth element */
template <class T>
int List<T>::max(int i,int j) {

  try {
    if (i < 0 || i >= size || j < 0 || j >= size)
      throw RangeError (__FILE__, __FUNCTION__, __LINE__, size);
  }
  catch (const RangeError &R)
  {
    cerr << R;
    throw R;
  }

  if (list[i] > list[j]) { 
      return i; 
  }
  else { 
      return j; 
  }
}

/* optimized ascending bubble-sort algorithm */
template <class T>
void List<T>::sort() {
  for ( int i = 1; i < size; i++ )    // or <= size -1
         for ( int j = 0; j < size-i; j++ )    
            if ( list[j] > list[j+1] )      
               swap(j,j+1);
}

template <class T1>
ostream & operator << (ostream & out, List<T1> & l)
{
  for (int i = 0; i < l.size; i ++) {
       out << setw(5) << l.list[i] << " ";
  }
  cout << endl;
}
#endif
