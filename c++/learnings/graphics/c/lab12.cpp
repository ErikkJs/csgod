// Erik Soto
// 4/19/18
//
//CMPS 3350
//lab12.cpp
//
//
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <fstream>
using namespace std;

//frequency definitions
#define A4 440.0
#define F0 A4
#define F0_note A
#define F0_octave 4

enum note_t {
    C = 1,
    Cs = 2,
    D = 3,
    Ds = 4,
    E = 5,
    F = 6,
    Fs = 7,
    G = 8,
    Gs = 9,
    A = 10,
    As = 11,
    B = 12,
    END = B,
    HALF_STEPS_PER_OCTAVE = B 
};

void unitTest(double * arr);
void load(double *arr);
double freq(note_t note, int octave_delta);


int main(int argc, char *argv[]  )
{
    note_t note;
    int octave_delta;

    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <NOTE>  <OCTAVE_DELTA>" << endl;
        return 0;
    }
    //
    note = (note_t)(toupper(argv[1][0]) - 64);
    switch(toupper(argv[1][0])) {
        case 'A': note = A; break;
        case 'B': note = B; break;
        case 'C': note = C; break;
        case 'D': note = D; break;
        case 'E': note = E; break;
        case 'F': note = F; break;
        case 'Z': note = C;
                  if((int)argv[1][1] == 0){
                      double comp[120];
                      load(comp);
                      unitTest(comp);
                      cout << "third " << argv[3][1] << endl;
                  }

    }
    //You may call your unit test here...



    if (note > END) {
        cout << "Invalid note!" << endl;
        return 1;
    }
    octave_delta = atoi(argv[2]);
    cout << HALF_STEPS_PER_OCTAVE << endl;
    cout << "Frequency: "<< freq(note, octave_delta) << endl;
    return 0;
}

//-----------------------------------------------------------
//Generate a frequency in hz that is half_steps away from C_4
//Do not modify this function.
//-----------------------------------------------------------
double freq(note_t note, int octave_delta) {
    double freq;
    double factor;
    double a;
    int n;
    int octave = octave_delta - F0_octave;

    a = pow(2.0, 1.0/(double)HALF_STEPS_PER_OCTAVE);
    n = note - F0_note;
    freq = F0 * pow(a, (double)n);
    factor = pow(2.0, (double)octave);
    freq = freq * factor;
    return freq;
}
void load(double *arr) {
    string s;
    ifstream data("data.txt");
   
    if(data.fail())
    {
        cout << " ERRROR" << endl;
    }

    int i =0;
    while(data >>s){
       double d = atof(s.c_str());
        arr[i] = d;
        cout << d;
        cout << arr[i] << endl;
    }
    data.close();


}

void unitTest(double *arr) {

    cout << "Note  " << "Octave" << "  value" << "    diff" << endl;
    cout << "----  ------  -----    ----" << endl;   
    for(int i =0; i<9; i++)
    {
        for(note_t test = C; test <= END; test = note_t(test +1)){ 
            string n;
            double f = freq(test,i);
            double mf = arr[i];
            double diff = mf-f;
            cout << diff;
            switch(test) {
                case 1: n = "C"; break;
                case 2: n = "Cs"; break;

                case 3: n = "D"; break;

                case 4: n = "Ds"; break;

                case 5: n = "E"; break;

                case 6: n = "F"; break;

                case 7: n = "Fs"; break;

                case 8: n = "G"; break;

                case 9: n = "Gs"; break;

                case 10: n = "A"; break;

                case 11: n = "As"; break;

                case 12: n = "B"; break;
            }
            // the note
            cout << n;
            // cout << HALF_STEPS_PER_OCTAVE << endl;
            // Octave
            if(test == 2 || test == 4 || test == 7 || test == 9 ||test == 11)
            {  
                cout  << setw(7) << i << setw(11) << f;
                cout << setw(15) <<diff << endl; 
            
            }
            else{    
                cout  << setw(8) << i << setw(11) << f;
                cout << setw(15) <<diff << endl; 
            }
        }

    }

}


