#include "GameLevel.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

class GameLevel{
    private:
        int _pipe_count;
        int * _pipes; //stores height of bottom pipe

        int _gap; //gap above  the bottom pipe
    public:
        static int world_height = 20;

        //Default constructor - use to set
        GameLevel()
        {
            _pipes = new int [6];

        }
        //copy constructor
        GameLevel(const GameLevel &)
        {
            this->_pipe_count = _pipe_count;
            for (int i =0; i<_pipe_count; i++)
            {

                this->_pipes[i] = _pipes[i];
            }
        }

        //destructor
        ~GameLevel()
        { 
            delete _pipes;

        }
        //load the level ingo form a file
        void load(char *file )
        {
            int num;
            ifstream ifs(file);
            if(ifs.is_open())
                while(!ifs.eof())
                {
                    int i=0;
                    ifs >> num;
                    _pipes[i] = num;
                    i++;
                    _pipe_count++;
                }
        }

        // set the gap
        void set_gap(int gap) 
        {
            _gap = gap;
        }

        // shows the pipes
        friend ostream & operator <<( ostream & show , GameLevel & level)
        {
            int count = 0;
            int n=0;
            int bottom;
            while(count!= level._pipe_count)
            {
                bottom = level._pipes[n];

                for (int i = 0; i <bottom; i++)
                {
                    show <<  "I"; 
                }
                for ( int j=0 ; j< level._gap; j++)
                {
                    show  << " ";
                }


                int top;
                top = level.world_height - level._gap - bottom;

                for ( int k = 0; k< top; k++)
                {
                    show << "I";

                }


                for(int i =0; i <rand()%2+2; i++)
                {
                    show << endl;
                }
                count++;
                n++;
            }
            return show;
        }
        // changes the hieghts 
  
        void adjust_heights(int delta)
        {
            for( int i =0; i < _pipe_count; i++)
            { _pipes[i] = _pipes[i] + delta;
            }
            // overload the << operator to output the pipes
            // you can/should use a non-overloaded show() function
            // first to test everything is working ";

        }

};






