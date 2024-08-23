#include <iostream>
#include <time.h>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;


int main() {
    const int ROWS = 100;
    const int COLS = 4;
    const int BUFFSIZE = 80;


    float data[ROWS][COLS];
    float labels[ROWS];
    float labels_two[ROWS];
    float labels_three[ROWS];
    char buff[BUFFSIZE];

    ifstream file("iris-binary-normalized.csv");

    file.good() ? cout << "File successfully opened\n" : cout << "ERROR: file did not open\n";


    string line;

    int col = 0;
    int row = 0;
    int y = 0, c = 0, er = 0;
    int yd;
    int error;
    double charge = 0;
    double alpha = 0.05;
    double weight[3];

    srand(unsigned(time(NULL)));

    //randomizing weights
    for (int i = 0; i < 4; i++) {
        weight[i] = ( (double)rand()/(RAND_MAX)) - 0.5;
        cout << "weight " << i << " : " << weight[i] << endl;
    }

    double theta = ((double)rand()/(RAND_MAX) ) - 0.5;

    //while there are still lines in file
    while ( getline(file, line)) {
        istringstream stream(line);
        string result;

        while ( getline (stream, result, ',') ) {
            if (col < 4) {
                data[row][col] = atof(result.c_str() ); //make into double
                cout << "row: " << row << "\tcol: " << col << "\tvalue: " << data[row][col] << endl;
                col++;
            } else {
                labels[row] = atof(result.c_str() ); //predicted
            }
        }
        row++;
        col = 0;
    }

    cout << "it me\n";


    // ACTUAL LAB
    // ----------------------------------------------------
    for(int epochs=0; epochs<200; epochs++){
       c=0;
        for (int i = 0; i < ROWS; i++) {
            charge = 0;
            er++;
            for (int j = 0; j < COLS; j++) {
                charge += data[i][j] * weight[j];
                //Step function 
                charge >= theta ? y = 1 : y = -1;
            }
            yd = labels[i];
            error = labels[i] - y;
            if ( error != 0 ) {
                for (int j = 0; j < COLS; j++) 
                    weight[j] = weight[j] + alpha * data[i][j] * error;
                theta = theta + alpha * (-1) * error;
                cout << "interation " << epochs << ": Weights are [" <<
                    weight[0] << " " << weight[1] <<  " " << weight[2] <<  " " << weight[3] << "] class has an error!" <<endl;
                    cout << "weights will be modified\n";
            } else {
                cout << "interation " << epochs << ": Weights are [" <<
                weight[0] << " " << weight[1] <<  " " << weight[2] <<  " " << weight[3] << "] class was correct!" <<endl;
                c++;
            }
        }
    }


    cout << c << " / " << er << endl;
    float correct = c/er;
    cout << "correct percentage: " << correct << endl;
    
    return 0;
}

