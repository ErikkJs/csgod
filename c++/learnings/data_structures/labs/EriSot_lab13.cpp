#include <iostream>

using namespace std;

// declare function pointers 

// this function pointer can point to a function
// that performs comparisons
typedef bool (*comparer)(int, int);

// this function pointer can point to a function
// that sorts an array, using a comparison function
// passed in the third parameter 
typedef void (*sorter)(int *, int, comparer);

int const SIZE = 20;
int UNSORTED[SIZE] = { 15, 1, 3, 25, 8, 11, 9, 34, 17, 12,
                       7, 18, 2, 14, 19, 50, 23, 26, 16, 10 };

class QuadSort {
    public:
        int data[SIZE];

        // this is a pointer to a sort function
        sorter sort;

        QuadSort() 
        {
            sort = NULL;
        }

        void show_data() 
        {
            for (int i = 0; i < SIZE; i++)
            {
                cout << data[i] << " ";
            }
            cout << endl;
        }
};

bool increasing(int, int);
bool decreasing(int, int);

// for fun, attempt a strange comparison algorithm?
bool strange(int, int);

void selection_sort(int *, int, comparer);
void insertion_sort(int *, int, comparer);
void bubble_sort(int *, int, comparer);
// EXTRA CREDIT
// Although, technically, mergesort is not a quadratic sort
// the QuadSort class won't know or care that it's calling merge_sort
// void merge_sort(int *, int, comparer);

void copy_array(int *, int *, int);

int main()
{
    QuadSort quad;

    // copy the unsorted data into our temp array
    copy_array(UNSORTED, quad.data, SIZE);

    // now call the selection sorter ------------------------------
    quad.sort = selection_sort;
    quad.sort(quad.data, SIZE, increasing);

    // display data
    cout << "\nSorted using Selection: " << endl;
    quad.show_data();

    // now call the insertion sorter ------------------------------
    // uncomment the line below if you want to use unsorted data
    // for your insertion sort. If you don't, your data will already
    // have been sorted by selection above
    // copy_array(UNSORTED, quad.data, SIZE);
    quad.sort = insertion_sort;
    quad.sort(quad.data, SIZE, decreasing);

    // display data
    cout << "\nSorted using Insertion: " << endl;
    quad.show_data();

    // now call the bubble sorter --------------------------------
    quad.sort = bubble_sort;
    quad.sort(quad.data, SIZE, increasing);

    // display data
    cout << "\nSorted using Bubble: " << endl;
    quad.show_data();

    return 0;
}

void copy_array(int *src, int *dest, int size)
{
    for (int i = 0; i < size; i++)
    {
        *(dest + i) = *(src + i);
    }
}

void selection_sort(int * array, int size, comparer fncompare)
{
    cout << "NOT IMPLEMENTED YET\n";
    // implement the selection sort algorithm
    // be sure to use the fncompare() function to do any
    // comparisons to determine sort order (see bubble_sort example)
}

void insertion_sort(int * array, int size, comparer fncompare)
{
    cout << "NOT IMPLEMENTED YET\n";
    // implement the insertion sort algorithm
    // be sure to use the fncompare() function to do any
    // comparisons to determine sort order (see bubble_sort example)
}

void bubble_sort(int * array, int size, comparer fncompare)
{
    // implement the bubble sort algorithm
    int temp;
    int i = 0;
    bool swapped;
    while (i < size-1 && swapped)
    {
        swapped = false;
        for (int j = 0; j < size-1; j++)
        {
            if (fncompare(array[j+1], array[j]))
            {
                temp = array[j+1];
                array[j+1] = array[j];
                array[j] =  temp;

                // set flags to continue swapping
                swapped = true;
                i = 0;
            }
        }
        i++;
    }
}

bool strange(int lhs, int rhs)
{
    // here's an idea for a 'strange' sort
    // take lhs, and get the 1st digit of the number
    // for example, if it's 900, the 1st digit is 9
    // also take the 1st digit of rhs
    // for example, if it's 600, the 1st digit is 6
    // now, map 9 (lhs) to its english word => 'nine'
    //      map 6 (rhs) to its english word => 'six'
    // take the 1st character of 'nine' => 'n'
    // take the 1st character of 'six'  => 's'
    // now do an alphabetized sort on the 1st character
    // of lhs versus the 1st character of rhs
    // in this example, 'n' IS less than 's'
    // which would mean that for this strange comparison
    // 900 is less than 600
    return true;
}

bool increasing(int lhs, int rhs)
{
    return lhs < rhs;
}

bool decreasing(int lhs, int rhs)
{
    return lhs > rhs;
}


