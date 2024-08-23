#include <iostream>

using namespace std;

struct ListNode{
    string name;
    ListNode * next;
};

int main ()
{
    // create a head pointer 
    ListNode * head;

    // create a new node (name = Walter)
    ListNode *node1 = new ListNode;
    node1->name = "Walter";
    // create a new node (name = Jesse)
    ListNode * node2 = new ListNode;
    node2->name= "Jesse";
    // link head -> Walter->Jesse->NULL
   node1 -> next = node2;
   node2 -> next = NULL;
   head = node1;
    //Display the list
    ListNode *temp = head;
    while(temp != NULL)
    {
      cout << temp -> name << " "; 
        temp = temp -> next;
    }
  cout << endl;  

   delete node1;
   delete node2;
   delete temp;
    return 0;
}

