// Erik Soto
// Feb 11 2016
// lab 4

#include <iostream>

using namespace std;

class Storage{
    protected:
        unsigned int _capacity;
        unsigned int _used;
        int get_free_space();
        string name;
    public:
        Storage()
        {
            name = "FIXED"; // sets name 
        }

        void set_cap(int cap)
        {
            _capacity = cap; // sets cap
        }

        void set_used(int used)
        {
            _used = used; // sets used 
        }

        int show_capcity()
        {
            return _capacity; //returns _cap

        }
        int show_used()
        {
            return _used;  // return _used 
        }
        int show_free()
        {
            int yeet;

            yeet = _capacity - _used;

            return yeet;     // returns the free memory
        }
        string get_name()
        {
            return name; // returns the name
        }

};

class Removable: public Storage {
    public:
        bool is_ready;

        Removable(){
            is_ready = false;  // sets the is_redy to false

        }
        void eject()
        {
            is_ready = false;  // sets to false 
        }


        string show_isready()  // checks to see what ready is to return a string 
        {
            if(is_ready == true)
            {
                return " is ready";
            }
            if(is_ready== false)
            {
                return " is not ready";
            }
        }

};

class Optical : public Removable{
    public:

        Optical(){   // constructor 

            set_cap(700);
            name = "OPTICAL";

        }
        void load(){   // sets to true 

            is_ready = true;

        }
};

class Flash: public Removable{
    public:
        Flash()
        {
            name = "FLASH DRIVE";  // sets name
        }

        void plug_in()  // sets the is_ready to true
        {
            is_ready = true;

        }


};
// global initiations 
void show_state(Removable);
void show(Removable);
void show(Storage);
int main()
{
    Storage hd;
    hd.set_cap(1024);
    hd.set_used(128);
    show(hd);

    Optical cd;
    cd.load();
    show_state(cd);
    cd.set_used(400);
    show(cd);
    cd.eject();
    show_state(cd);

    Flash usb;
    usb.set_cap(512);
    usb.set_used(12);
    usb.plug_in();
    show_state(usb);
    show(usb);
    usb.eject();
    show_state(usb);

    return 0;
}

// shows the storage 
void show(Storage s)
{
    cout << s.get_name()  << " has " << s.show_capcity() << "MB capacity" << s.show_used() << "MB used, " << s.show_free() << "MB free" << endl;
}


void show_state(Removable s) // returns the state
{

    string yeet;
    yeet = s.show_isready();

    cout << s.get_name() << yeet << endl;
}

void show(Removable r)  // shows the capacity and stuff 
{

   
 cout << r.get_name() << " has " <<  r.show_capcity() << "MB capacity " << r.show_used() << "MB used, " << r.show_free() << "MB free " << r.show_isready() << endl;    
}




