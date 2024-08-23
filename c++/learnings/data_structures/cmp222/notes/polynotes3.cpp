#include <iostream>

using namespace std;

class Organ{
    protected:
        void organize() {

        }
};

class Heart : public Organ {
    public:
        int valves;
        void pump(){}

};

class Brain : public Organ{
    public:

        int oxytocin;
        void trigger_fight()
        {

        }

        void trigger_flight()
        {
        }

        void control_organ(Organ) {
        }
};

class ILiving {
    protected:                      // interface    
        virtual void die() =0;      // class
        virtual void spawn() =0;    // cant instantiate a "Living"
        virtual void grow() =0;     // must have the "I" at beginning 

};

class Flora : public ILiving {
    public: 
        void die() {}
        void spawn() {}
        void grow (){}
};

class Fauna : public Living{
    public: 
        Heart heart;
        Brain brain;

        void die(){}
        void spawn() {}
        void grow() {}

};

class Sentient{
    public:
        void think(){}
        void feel () {}
};

class Human : public Fauna , public Sentient{
};

int main()
{
 Human human;

 human.spawn();
 human.think();
 human.feel();
 human.grow();
 human.die();
 human.heart.pump();
 human.brain.trigger_flight();
 human.brain.trigger_fight();

return 0;
}


