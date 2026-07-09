// demonstrates file IO and command-line input in C++
#include <fstream>   // for ifstream and ofstream
#include <iostream>

using namespace std;

int main ( int argc, char *argv[] )   // support for command-line args
{
  char str[10];

  if ( argc != 2 ) // argc holds no. of args - the first is the executable name
  {
     cout << "Usage: "<< argv[0] <<" <filename>\n";  // usage statement
     return 0;
  }

  ofstream outf( argv[1] );  // open an external file for output
  if ( !outf.is_open() ) {
     cout<<"Could not open file\n";
     return 0;
  }

  /* redirect cerr to the output file */
  streambuf *buf = cerr.rdbuf();   // keep the cerr buffer to restore it
  cerr.rdbuf( outf.rdbuf() );      // redirect cerr 
  outf << "Write this text to data.txt\n";
  cerr << "This should go to the same place ...\n";

  /* some other ways to open files */
  ofstream outf2 ( "data.txt", ios::app );  
  outf2 << "\nAppend this text to data.txt";
  outf2.close();
 
  ifstream inf ( "data.txt" );  // opens infile.txt for reading 
  if ( !inf.is_open() ) 
      cerr << "The file could not be opened\n";
 else {
     inf >> str; // read a string from the file
     cout << str <<"\n";
  }
  //  cin.get();    // do this to wait for a keypress
  // data.txt is closed implicitly here
  cerr.rdbuf(buf);
  return 0; 
}
