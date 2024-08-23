#ifndef ANIMAL_H
#define ANIMAL_H

using namespace std;

enum Diet  { CARNIVORE, HERBIVORE, INSECTIVORE, OMNIVORE};

class Animal {
    private:
        Diet _diet;
        string _name;
    public:
        Animal(string, Diet);
        string get_name();
};
#endif
