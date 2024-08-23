//Erik Soto
//cmp 223
// hw 1 
// 4-13-16
#include <iostream>

using namespace std;

struct ListNode{
    int value;
    ListNode * next;

};

// makes a node 
ListNode *createNode(int elem)
{
    ListNode * ptr;
    try
    {
        ptr = new ListNode;
        ptr -> value = elem;
        ptr->next = NULL;
        return ptr;
    }
    catch(bad_alloc)
    {
        ptr = NULL;
    };


}

// inserts a node 
ListNode * listInsert(ListNode *head, ListNode * prev , int elem)
{
    ListNode *node = createNode(elem);

    if(head == NULL)
    {
        prev=node;
        head=node;
        return head;
    }
    else
    {
        prev->next=node;
        return node;
    }

}

// loocks for a elemetn in a node
ListNode * listSearch(ListNode *head, int elem)
{
    ListNode *temp;
    temp = head;
    if(head !=NULL)
    {
        while(temp->value != elem)
        {
            if(temp->value == elem)
            {
                cout << "found" << endl;

                return temp;
            }

            temp = temp->next;
        }
    }

    cout << " NOT FOUND " << endl;
    return NULL;

}

// deletes a node
ListNode * listDelete(ListNode * head , int elem)
{


    ListNode *temp;
    temp = listSearch(head,elem);

    ListNode * prev = new ListNode;


    while(prev->next==temp)
    {
        if(prev->next==temp)
        {
            prev->next=temp->next;
        }
        else
            prev= prev->next;
    }


}
// shows the values in the node
void listShow(ListNode * head)
{
    ListNode *temp;
    temp = head;

    if(head!=NULL)
    {
        while(temp !=NULL)
        {
            cout << temp->value<< " ";
            temp = temp->next;
        }
    }
    else 
        cout << "List Empty" << endl;
}

// deallocates the node
void listDeallocate(ListNode *head)
{
    ListNode * temp;
    temp = head;
    while(temp != NULL)
    {
        head = head->next;
        delete temp;
        temp = head;
    }

}
// menu
int menu()
{
    int i;
    do {
        cout << "______________________________________" << endl;
        cout << " 1 - Search for an element in the list " << endl;
        cout << " 2 - Add an element to the list " << endl;
        cout << " 3 - Delete an Element from the list " << endl;
        cout << " 4 - Show the list " << endl;
        cout << endl; 
        cout << " 0 - Exit" << endl;

        cin >> i;
        return i;
    }
    while( i < 0 || i >4);
}

int main ()
{
    ListNode * head = NULL;
    ListNode * prev= NULL;

    int choice;
    do
    {
        choice = menu();
        switch(choice)
        {
            case 1:
                int value;
                int found;
                cout << " what element are you looking for" << endl;
                cin >> value;
                listSearch(head,value);


                break;
            case 2:
                int value2;

                cout << " what element do you want to add" << endl;
                cin >> value2;

                if( prev == NULL)
                {
                    head=listInsert(head,NULL,value2);
                    prev = head;
                }
                else 
                    prev = listInsert(head,prev,value2);

                break;
            case 3:
                int value3;

                cout << " what element do you to delete " << endl;
                cin >> value3;

                listDelete(head , value3);
                break;
            case 4:
                listShow(head);
                break;
            case 5:
                listDeallocate(head);
                break;
        }
    }
    while(choice != 0);

    return 0;

}

