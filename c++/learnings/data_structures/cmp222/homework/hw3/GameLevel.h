#ifndef  GAMELEVEL_H
#define GAMELEVEL_H

class GameLevel{
    private:
        int _pipe_count;
        int * _pipes; //stores height of bottom pipe
        
        int _gap; //gap above  the bottom pipe
    public:
        static int world_height;

        //Default constructor - use to set
        GameLevel();
        //copy constructor
        GameLevel(const GameLevel &);
        
        //destructor
        ~GameLevel();
             
       //load the level ingo form a file
            void load(char* filename );
        // set the gap
        void set_gap(int); 
        
        
        //adjust the bottom pipe heights 
        //this function will add delta to each value in
        //the _pipes array. if a negative value of delta 

        void adjust_heights(int);

};

#endif
