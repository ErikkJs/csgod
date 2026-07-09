//
// modified by:erik soto
// date:
//
// Lab-6 program code.
//
// The goal of this program is to read through a file of dictionary words
// and get a count of all words in which the 4th letter is an a.
//
// The file you are processing is named cracklib-small.
// It's located in the dict folder of the share folder in the usr directory.
//  --> Do not make your own copy of this file.
//  --> Leave it in-place.
//  --> This is a standard dictionary file on Unix and Linux systems.
//  --> Do not make your own local copy.
//
//
// This source code contains errors and bugs that should be corrected.
// The program also has poor style and poor qualities of integrity.
// Fix everything please.
//
//
// Things to do
//
//  on a single sheet of paper...
//  1. List the bugs that you find.
//  2. Describe the method you used to find it.
//  3. Describe how you fixed it.
//  4. Describe how you tested the fix.
//
// You may refer to our textbook named:
// Effective Debugging: 66 Specific Ways to Debug Software and Systems
// on Safari
//
//
#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

//Fixed the mispelling of the file name
const char *filename = "/usr/share/dict/cracklib-small";

int main() {

   cout << "-------------------------------------\n";
	cout << "This is a lab-6 program for CMPS-3350\n";
	cout << "-------------------------------------\n";

   //Define an input file stream, open and read.
	ifstream fin;
	fin.open(filename, ios::in);
   // changed to if fin.fail() from !fin.fail() 
   if(fin.fail()){
		cout << "Error opening: " << filename << " for output." << endl;
    	exit(1);
	}
	cout << "Reading the file...\n";
	char letter = 'a';
   //handled the warning by getting rid of the unused variable, used the all warning compiling setting
	int count =0;
   // changed from a pointer to an array for practicality
   char word[30];
	fin >> word;
   // fixed the while statement, the logic was incorrect making it impossible to enter.
	while(!(fin.eof()))
	{
       // subscript was incorrect changed to a 3 from 4. used the logging functions from cmpslib.h
		if(word[3]== letter)
		    count += 1;	
      word[3]=0;
      fin >> word;

	}
	cout << "The program has finished.\nThe count of words with 4th letter 'a' is: " << count << endl;	
	cout << endl;
	return 0;
}


