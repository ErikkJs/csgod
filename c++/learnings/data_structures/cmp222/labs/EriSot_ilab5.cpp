// erik soto
// Feb 18
// ilab4

#include <iostream>

using namespace std;

class Gun{
    protected:
        string fire_rate;
        string damage;
        string range;
        string handling;
        string mobility;
        string accuracy;
        int magazine_size;
};

class Assault_Rifle{

    Assault
    {
        magazine_size = 30;
    }

};

class Primary{
    virtual void shoot() = 0;
    virtual void reload() = 0;
    virtual void aim() = 0;
};

class Secondary{

    virtual void shoot() = 0;
    virtual void reload() = 0;
    virtual void aim() = 0;
};

class Ak47 :public Assault_Rifle , public Primary , public Gun{
    
    AK47{
    fire_rate = "|||||||||";
    damage = "||||||||||||";
    range = "||||||||";
    handling = "|||||||";
    mobility = "||||||";
    accuracy = "||||||||||||";
    }

    void shoot(){

        magazine_size = 20;
    }

    void reload(){

        magazine_size =30;

    }

    void aim() {

        cout << "aiminggggggg" << endl;
    }
};

class G36e : public Primary , public Gun,
public Assault_Rifle{

    G36e{
    fire_rate = "|||||||||||";
    damage = "||||||||||";
    range = "||||||";
    handling = "||||||";
    mobility = "|||||||||";
    accuracy = "||||||||";
    }

    void shoot(){

        magazine_size =20;
    }

    void reload(){

        magazine_size =30;

    }

    void aim() {

        cout << "aiminggggggg" << endl;
    }
};

class M16 : public Primary , public Gun,
public Assault_Rifle{
    
    M16{
    fire_rate = "|||||||||||";
    damage = "|||||||";
    range = "||||||||||";
    handling = "|||||||||||";
    mobility = "||||";
    accuracy = "||||||||||";
    }
    void shoot(){

        magazine_size =20;
    }

    void reload(){

        magazine_size =30;

    }

    void aim() {

        cout << "aiminggggggg" << endl;
    }
};
class AUG : public Primary , public Gun,
public Assault_Rifle{

    AUG{
    fire_rate = "||||||||||||";
    damage = "||||||||";
    range = "|||||||||||";
    handling = "|||||";
    mobility = "|||||||||";
    accuracy = "|||||||";
    }
    void shoot(){

        magazine_size =20;
    }

    void reload(){

        magazine_size =30;

    }

    void aim() {

        cout << "aiminggggggg" << endl;
    }
};
class Scar-H : public Primary , public Gun,
public Assault_Rifle{

    Scar-H{
    fire_rate = "||||||||||||";
    damage = "||||||";
    range = "|||||||||";
    handling = "|||||||||||";
    mobility = "|||||||||";
    accuracy = "|||||||||";
    }
    void shoot(){

        magazine_size = 20;
    }

    void reload(){

        magazine_size =30;

    }

    void aim() {

        cout << "aiminggggggg" << endl;
    }
};
class M4a1: public Primary , public Gun, public Assault_Rifle{
   
    M4a1{
    fire_rate = "|||||||||";
    damage = "||||||||||||";
    range = "||||||||";
    handling = "|||||||";
    mobility = "||||||";
    accuracy = "||||||||||||";
    }
    void shoot(){

        cout <<  " ALLAHHHHHHHHHH AKBARRRRRR" << endl;
    }

    void reload(){

        magazine_size =30;

    }

    void aim() {

        cout << "aiminggggggg" << endl;
    }
};
class Mtar21 : public Primary , public Gun, public Assault_Rifle{

  Mtar21{
      fire_rate = "|||||||||";
    damage = "||||||||||||";
    range = "||||||||";
    handling = "|||||||";
    mobility = "||||||";
    accuracy = "||||||||||||";
  }
    void shoot(){

        cout <<  " ALLAHHHHHHHHHH AKBARRRRRR" << endl;
    }

    void reload(){

        magazine_size =30;

    }

    void aim() {

        cout << "aiminggggggg" << endl;
    }
};
class Ak47 : public Primary , public Gun{

    fire_rate = "|||||||||";
    damage = "||||||||||||";
    range = "||||||||";
    handling = "|||||||";
    mobility = "||||||";
    accuracy = "||||||||||||";
    magazine_size = 30;

    void shoot(){

        cout <<  " ALLAHHHHHHHHHH AKBARRRRRR" << endl;
    }

    void reload(){

        magazine_size =30;

    }

    void aim() {

        cout << "aiminggggggg" << endl;
    }
};



int main () 
{
}

