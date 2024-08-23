#include <iostream>
using namespace std;

class Node
{
public:
    int data;
    Node *next;

    Node(int data) : data(data), next(nullptr) {}
};

class SingleLinkedList
{
public:
    Node *head;
    Node *tail;

    SingleLinkedList() : head(nullptr) {}

    void insertAtHead(int data)
    {
        Node *newNode = new Node(data);
        newNode->next = head;
        head = newNode;
        tail = newNode;
    }

    void insertAtTail(int data)
    {
        Node *newNode = new Node(data);

        if (head == nullptr)
        {
            head = tail = newNode;
            return;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
    }
    void display()
    {
        Node *current = head;
        while (current != nullptr)
        {
            std::cout << current->data << endl;
            current = current->next;
        }
    }
    ~SingleLinkedList()
    {
        Node *current = head;
        while (current != nullptr)
        {
            Node *next = current->next;
            delete current;
            current = next;
        }

        head = nullptr;
    }
};

int main()
{
    SingleLinkedList list;
    cout << "Inserting 10 at head" << endl;
    list.insertAtHead(10);
    list.insertAtTail(5);
    list.display();
    return 0;
}