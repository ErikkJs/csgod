//erik soto
//homework 2
// 1/26/2016

#include <iostream>

using namespace std;


class TennisPlayer{
    private:
        int _games;
        int _sets;
    public:
        bool setdone;
        // gets the sets 
        int get_sets(){


            return _sets;}
        // gets the games 
        int get_games() {

            return _games; }

            // resets all games to 0
        int reset_games() {

            _games = 0;}

//overloaded to add to the players w's
        TennisPlayer operator ++(int)
        {
            TennisPlayer _games;

            this-> _games = get_games();
            this->_games++;

                setdone = false;
                cout << "games : " << this->_games << endl;
            if(this->_games ==4)
            {
               this-> setdone = true;
               this-> _games = 0;
              this->  _sets = _sets +1;
            }



        }
};

class TennisMatch{
    private:
    public:
        TennisPlayer player1;
        TennisPlayer player2;
       //checks for winners 
        bool in_progress()
        {   
            return ( player1.get_sets() !=2 &&
                    player2.get_sets() !=2 );
        }
        void reset_games()
        {
            // if one player completes a set, reset the other to zero

            if(player1.setdone)
                player2.reset_games();
                player1.setdone = false;
            if (player2.setdone)
                player1.reset_games();
                player2.setdone = false;
        }

        //over loads the cout thingy to be used in main more sexy
        friend ostream & operator << (ostream &out , TennisMatch &tp)
        {
            out <<"player 1: " << tp.player1.get_sets() << " player 2: "<< tp.player2.get_sets();

            return out;
        }
};

int main()
{
    TennisMatch match;
    int player;
    
    // checks to make sure anyone has won
    while (match.in_progress())
    {
        cout << "Who wins the next game (player 1 or 2)?";
        cin >> player;

        if(player ==1)
            match.player1++;
        if(player ==2)
            match.player2++;

        match.reset_games();
    }

    cout << " Game over, score is " << match << endl;

    return 0;
}


