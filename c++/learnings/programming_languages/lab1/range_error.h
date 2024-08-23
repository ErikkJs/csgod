/*
  Filename: range_error.h
  Purpose: a RangeError exception class 
*/

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <iostream>
#include <string>
#include <cstring>

using namespace std;

const unsigned FileNameSize = 40;
const unsigned FuncNameSize = 40;

//out of range subscript exception class
class RangeError {

  friend ostream & operator <<( ostream & os, const RangeError & err)
  { 
    os << "RangeError exception thrown, file: "
       << err.fileName
       << err.funcName 
       << ", lineno: " << err.lineNumber
       << ", index: " << err.value
       << endl;
       return os;
  }

  public:

    RangeError( const char *fn, const char *funcName,  int line, int subscr)
    {
      strcpy(fileName, fn);
      lineNumber = line;
      value = subscr;
    }
   
    // a standard method for all range error classes  -- returns error type
    const char * what () const
    {  
       return "RangeError Exception";
    }
    
  private:
    char fileName [FileNameSize + 1];   // __FILE__
    char funcName [FuncNameSize +1];
    int lineNumber;                     // __LINE__
    int value;                          // the out-of-range index 
};

#endif
