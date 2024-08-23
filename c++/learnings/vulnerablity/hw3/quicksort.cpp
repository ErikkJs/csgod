// Author:  Melissa Danforth
// Purpose: To perform the quicksort algorithm
//          In debug mode, this will print out each swap as it operates
//          using ANSI colors to highlight the swapped values
//          It will also pause briefly between runs in debug mode
//
// Note: We do not need to define swap() as it is already defined in the
//       included libraries

#ifndef _QUICKSORT_CPP_
#define _QUICKSORT_CPP_

#include <iostream>        // for the swap() function

#define THRESHOLD      10  // Below this value, insertion sort is used

#define DEBUG_MODE         // When defined, debug mode is enabled

#ifdef DEBUG_MODE          // Debug mode options:
#define ROW_ELEMENTS   20  // Number of elements output per row
#define ELEMENT_SPACE   4  // The setw() argument
#include <iomanip>         // to set the column widths
#include <stdio.h>         // for the printf function

#define SLEEP_USEC 2500000 // Microseconds to pause between output
#ifdef WIN32               // Windows does not have usleep function
#include <windows.h>       // So we have to wrap the Sleep function
#define usleep(x) Sleep((x)/1000)  
#else
#include <unistd.h>        // Unix has the usleep function
#endif // WIN32
#endif // DEBUG_MODE

using namespace std;

/********************* Function Prototypes *******************************/

// Median-of-three finds the median of the first, last and middle values
// Input: The list a, the first index and the last index
// Output: The index where the median value can be found
template <class T>
int median_of_three(T a[], int first, int last);

// Split puts the smaller elements to the left of the pivot and the larger
// elements to the right of the pivot
// Input: The list a, the first index and the last index
// Output: The index where the pivot is located
template <class T>
int split(T a[], int first, int last);

// Quicksort is a recursive function that divides the list into smaller
// sublists. When the list size falls below THRESHOLD, quicksort uses 
// insertion sort to sort the list
// Input: The list a, the first index and the last index
// Output: List a is sorted
template <class T>
void quicksort(T a[], int first, int last);

// Insertion sort grows the list, inserting each element in sorted order
// Input: The list a, the first index and the last index
// Output: List a is sorted
template <class T>
void insertionsort(T a[], int first, int last);


/********************** Function Bodies ********************************/


template <class T>
int median_of_three(T a[], int first, int last)
{
  int m = (first + last) / 2;

#ifdef DEBUG_MODE
  // Dump the data and highlight the three possible pivots in yellow

  // First indent the appropriate number of spaces
  int indent = (first % ROW_ELEMENTS) * ELEMENT_SPACE;
  if(indent) cout << setw(indent) << " ";

  // Now output the elements from first to last
  for(int k = first; k <= last; k++)
  {
    if(k == m || k == first || k == last) vt_printf(YELLOW_F);
    cout << setw(ELEMENT_SPACE) << a[k];
    if(k == m || k == first || k == last) vt_printf(RESET);
    if( (k + 1) % ROW_ELEMENTS == 0 ) cout << endl;
  }
  cout << endl;
  if(SLEEP_USEC) usleep(SLEEP_USEC);
#endif

  // Put the three candidates in sorted order so the smallest is in index
  // first, the median is in index m and the largest is in index last
  if(a[m] < a[first])
    swap(a[m], a[first]);
  if(a[last] < a[first])
    swap(a[last], a[first]);
  if(a[last] < a[m])
    swap(a[last], a[m]);

  // Now index m has the median, return it as the pivot
  return m;
}

template <class T>
int split(T a[], int first, int last)
{
  // First call median of three to calculate the pivot
  int p = median_of_three(a, first, last);
  T pivot = a[p];

  int i = first + 1;  // searches forwards for elements > pivot
  int j = last;       // searches backwards for elements <= pivot

  // Move the pivot into the first slot since median-of-three did not swap 
  // the middle element (the pivot) to first position
  swap(a[first], a[p]);

#ifdef DEBUG_MODE
  // Dump the list before partitioning into smaller and larger

  // First indent the appropriate number of spaces
  int indent = (first % ROW_ELEMENTS) * ELEMENT_SPACE;
  if(indent) cout << setw(indent) << " ";

  // Now output the elements from first to last
  for(int k = first; k <= last; k++)
  {
    if(k == first) vt_printf(CYAN_F);
    cout << setw(ELEMENT_SPACE) << a[k];
    if(k == first) vt_printf(RESET);
    if( (k + 1) % ROW_ELEMENTS == 0 ) cout << endl;
  }
  cout << endl;
  if(SLEEP_USEC) usleep(SLEEP_USEC);
#endif

  // Swap elements to position smaller to left of pivot and larger to right
  while(i < j)
  {
    while(a[j] > pivot) j--;  // search backwards for a[j] <= pivot
    while(i < j && a[i] <= pivot) i++;  // search forwards a[i] > pivot
    // Found two elements that need to be swapped
    if(i < j) 
    {
#ifdef DEBUG_MODE
      // Highlight the two positions about to be swapped

      // First indent the appropriate number of spaces
      if(indent) cout << setw(indent) << " ";

      // Now output the elements from first to last
      for(int k = first; k <= last; k++)
      {
        if(k == first) vt_printf(CYAN_F);
        if(k == i || k == j) vt_printf(GREEN_F);
        cout << setw(ELEMENT_SPACE) << a[k];
        if(k == first || k == i || k == j) vt_printf(RESET);
        if( (k + 1) % ROW_ELEMENTS == 0 ) cout << endl;
      }
      cout << endl;
      if(SLEEP_USEC) usleep(SLEEP_USEC);
#endif
      // Swap the positions
      swap(a[i], a[j]);
    }
  }
  // Finished swapping elements, place pivot in its sorted order
  // Note: Since we have the backwards search happening before the forwards
  // search in the code above, we will stop the code when j is on an element
  // less-than or equal to the pivot. So we swap the pivot with j.
  // Had we done the forwards search before the backwards search, we'd have 
  // stopped on an element greater than the pivot, so we'd have to swap with
  // j-1 in that case. Keep this distinction in mind when coding quicksort.
  swap(a[first], a[j]);

#ifdef DEBUG_MODE
  // Output the list after partitioning into larger and smaller

  // First indent the appropriate number of spaces
  if(indent) cout << setw(indent) << " ";

  // Now output the elements from first to last
  for(int k = first; k <= last; k++)
  {
    if(k == j) vt_printf(CYAN_F);
    cout << setw(ELEMENT_SPACE) << a[k];
    if(k == j) vt_printf(RESET);
    if( (k + 1) % ROW_ELEMENTS == 0 ) cout << endl;
  }
  cout << endl;
  if(SLEEP_USEC) usleep(SLEEP_USEC);
#endif

  // Return the index that the pivot is located in
  return j;
}

template <class T>
void quicksort(T a[], int first, int last)
{
  // If first is 0 and last is 9, then there are 10 elements. 
  // If first is 10 and last is 14, then there are 5 elements.
  // This means the size is last - first + 1.
  int size = last - first + 1;

#ifdef DEBUG_MODE
  cout << endl;
  cout << "Quicksort called for indices " << first 
       << " to " << last << endl;
#endif

  // 0 or 1 element lists are always sorted. There's nothing to do, so just
  // exit the function. This might happen in the recursive calls to quicksort
  // if the split function makes uneven 'left' and 'right' partitions.
  if(size <= 1) return;

  // Call insertion sort for smaller lists. Note: If you do not use insertion
  // sort for smaller lists, check for size==2 here and put the two elements 
  // into sorted order without calling split or recursively calling another 
  // quicksort, since it makes no sense to use quicksort for a two element list
  if(size < THRESHOLD)
  {
    insertionsort(a, first, last);
  }
  else
  {
#ifdef DEBUG_MODE
    // Output information about the sort
    cout << "  The candidate pivots for median-of-three are shown in ";
    vt_printf(YELLOW_F);
    cout << "yellow";
    vt_printf(RESET);
    cout << ".\n  The chosen pivot is show in ";
    vt_printf(CYAN_F);
    cout << "cyan";
    vt_printf(RESET);
    cout << ".\n  The elements that are being swapped in split are shown in ";
    vt_printf(GREEN_F);
    cout << "green";
    vt_printf(RESET);
    cout << ".\n";
    cout << endl;
#endif

    // Choose a pivot and partition into larger and smaller
    int p = split(a, first, last);
    quicksort(a, first, p - 1);  // sort the smaller elements
    quicksort(a, p + 1, last);   // sort the larger elements
  }

#ifdef DEBUG_MODE
  // Print the sorted list
  cout << endl;
  cout << "The sorted list for indices " << first << " to " << last << " is:\n";

  // First indent the appropriate number of spaces
  int indent = (first % ROW_ELEMENTS) * ELEMENT_SPACE;
  if(indent) cout << setw(indent) << " ";

  // Now output the elements from first to last
  for(int k = first; k <= last; k++)
  {
    cout << setw(ELEMENT_SPACE) << a[k];
    if( (k + 1) % ROW_ELEMENTS == 0 ) cout << endl;
  }
  cout << endl;
#endif
}

// List is going to start at first instead of 1 and end at last instead of n, 
// so must adjust the loops accordingly. 
template <class T>
void insertionsort(T a[], int first, int last)
{
  T tmp;
  int size = last - first + 1;

#ifdef DEBUG_MODE
  // Print sort information
  cout << "Insertion sort called for indices " << first 
       << " to " << last << endl;
  cout << "  The slot to be inserted is shown in ";
  vt_printf(RED_F);
  cout << "red";
  vt_printf(RESET);
  cout << " after it is inserted in sorted order.\n";
  cout << "  Elements not part of the current pass are shown in ";
  vt_printf(BLUE_F);
  cout << "blue";
  vt_printf(RESET);
  cout << ".\n";
  int indent = (first % ROW_ELEMENTS) * ELEMENT_SPACE;
#endif

  if(size <= 1) return;  // 0 or 1 element list is already sorted

  // Insert element at p. If we started at 1, we'd start with p = 2, but since 
  // we at starting at first, we start with p = first + 1. We go through the 
  // last slot, including the last slot, so go while p <= last
  for(int p = first + 1; p <= last; p++)
  {
    // Now we must scan backwards from p until first, making the hole to insert
    // the element at p into.
    tmp = a[p];
    int j;  // Must declare outside of the loop since we need it after the loop
    for(j = p; j > first; j--)
    {
      if(tmp >= a[j - 1]) break;  // The hole has been found, end the loop
      a[j] = a[j-1];  // Shift the elements over
    }
    // Fill in the hole with tmp
    a[j] = tmp;

#ifdef DEBUG_MODE
    // Output the list after inserting element p into its hole

    // First indent the appropriate number of spaces
    if(indent) cout << setw(indent) << " ";

    // Now output the elements from first to last
    for(int k = first; k <= last; k++)
    {
      if(k > p) vt_printf(BLUE_F);
      if(k == j) vt_printf(RED_F);
      cout << setw(ELEMENT_SPACE) << a[k];
      if(k > p || k == j) vt_printf(RESET);
      if( (k + 1) % ROW_ELEMENTS == 0 ) cout << endl;
    }
    cout << endl;
    if(SLEEP_USEC) usleep(SLEEP_USEC);
#endif
  }
}

#endif
