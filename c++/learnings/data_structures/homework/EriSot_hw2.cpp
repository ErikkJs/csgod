// erik soto
// april 16
// hw2

#include <iostream> 

using namespace std;


struct Window{
    string program_name;
    int mem_req;
    int mem_used;
    Window * next;
    Window * prev;
};

// starts the process
Window * start_process(Window * head , Window *current , string name)
{
    Window * yeet = new Window;

    try{


        if(head == NULL)
        {
            yeet->program_name = name;
            yeet->next = yeet;
            yeet->prev = yeet;
            return yeet;
        }
        else 
        {  
            Window * temp = head;

            while(temp->next != head)
            { 
                temp = temp->next;
            }
            yeet->program_name = name;
            yeet->next = head;
            yeet->prev = current;
            current->next=yeet;
            head->prev = yeet;

            return yeet;
        }

    }

    catch(bad_alloc)
    {

        yeet = NULL;


    }

    return NULL;


}

// finds the names process
Window * find_process(Window * head , string name)
{
    Window * yeet = head;

    while( yeet->program_name != name)
    {
        if(yeet->program_name == name)
        {
            return   yeet;
        }
        yeet = yeet->next;
    }

    return NULL;


}
// ends the names process
Window * end_process(Window *head, string name)
{
    Window * yeet;
    Window *nc;
    yeet = head->next; 

    while(yeet->program_name != name)
    {

        yeet = nc->next;

    }
    yeet->prev->next = yeet->next;
    yeet->next->prev = yeet->prev;

    nc= yeet->next;

    delete yeet;
    return nc;



}
// ends the current process
Window * end_process(Window * current)
{
    Window * yeet;

    current->prev->next= current->next;
    current->next->prev = current->prev;

    yeet = current->next;
    return yeet;
}

// kills all the process 
void kill_processes(Window * head)
{
    Window * temp = head->next;
    while(temp!=NULL)
    {
        Window * del = temp;
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
        temp =temp->next;
        delete del;
    }
    delete temp;


}

// menu 
int menu()
{
    int userchoice;

    cout << "1 - Launch New Program" << endl;
    cout << "2 - Close Current Program" << endl;
    cout << "3 - Close Current Named Program" << endl;
    cout << "4 - Show Next Program" << endl;
    cout << "5 - Show Previous Program" << endl;
    cout << "0 - SHUTDOWN" << endl;

    cin >> userchoice;

    return userchoice;


}

int main ()
{
    Window * head = NULL;
    Window * current=NULL;

    head = start_process(head, current , "Microsoft Word");
    current = head;
    current = start_process(head, current , "Halo");
    current = start_process(head, current , "Calculator");

    int ch;
    do{
        ch = menu();
        switch(ch)
        {


            case 1: // launch new program
                {
                    string pn;
                    cout << " Enter name of program you will open" <<endl;
                    cin >> pn;
                    current=start_process(head,current , pn);
                    if (current != NULL)
                    {
                        cout << " ADDED" << endl;
                    }
                    else 
                    {

                        cout << " YEEEEEEEEEEEEEET" << endl;
                    }


                }
                break;

            case 2: // close current program 
                {
                    current = end_process(current);
                    if(current != NULL)
                    {
                        cout << " ended" << endl;
                    }

                    else 
                        cout << " YEEEET" << endl;
                }
                break;


            case 3: // close current named program
                {
                    string nam;
                    cout << "what program you want to close" << endl; 
                    cin >> nam;
                    current =end_process(current,nam);

                    break;
                }
            case 4: // show next program

                current= current->next;
                cout << " next program is " << current->program_name << endl;

                break;

            case 5: // show previous program

                current = current->prev;
                cout << " the prev program is " << current->program_name << endl;
                break;

            case 0: // shutsdown deletes the nodes 
                  kill_processes(head);
                break;

        }

    }while(ch != 0);


    return 0;

}

