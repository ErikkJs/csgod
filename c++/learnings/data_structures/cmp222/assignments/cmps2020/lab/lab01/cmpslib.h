#pragma once 

#include <cxxabi.h>
#include <bitset>
#include <time.h>
#include <string>   // string class
#include <string.h> // c-string library
#include <sstream>  // ostringstream
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>  // c file output
#include <stdarg.h>
#include <typeinfo> // find names of ADTs at runtime
#include <limits.h> // min and max values for dataytypes
#include <limits>
#include <ios>
#include <float.h>
#include <math.h>
#include <iomanip>
#include <stdexcept>  // exception handling .. 
#include <signal.h>   // for catching the segfaults
#include <random>
using namespace std;



// this one is used by the prompt functions you wont need it
template <typename CharT>
std::streamsize ignore_line (std::basic_istream<CharT>& in, bool always_discard = false );



/**************************************
  user input
 ***************************************/
/*
   string PromptForString(string input)
   dispay the text 'input'

   put put a prompt
   take the text (up to 499 chars) and return it as a string
 */
inline string PromptForString(string input)
  {
  ignore_line( cin );// clear the buffer if needed
  while (true)
    {
    cout << input ;
    string user_input;
    getline(cin,  user_input);
    return user_input;
    }
  }



/* use cout to send the first parameter "text" to the console
   take in input from the user until they enter a valid integer
   value between min and max , the 2nd and 3rd parameters */
inline int PromptForInt( string text, int minval=INT_MIN, int maxval=INT_MAX)
  {
  string input;
  int valid_int;
  while (1)
    {
    input = PromptForString(text);
    try
      {
      valid_int = stod(input);
      if (valid_int < minval  || valid_int > maxval)
        {
        cout << "The entered value must be with the range " << minval << " to " << maxval << endl;
        }
      else
        {
        return valid_int;
        }

      }
    catch (...)
      {
      cout << "invalid value\n" << endl;
      ignore_line(cin);
      }
    }
  }


/* use cout to send the first parameter "text" to the console
   take in input from the user until they enter a valid double
   value between min and max , the 2nd and 3rd parameters */
inline double PromptForDouble( string text,double minval=DBL_MIN, double maxval=DBL_MAX)
  {
  string input;
  double valid_double;
  while (1)
    {
    input = PromptForString(text);
    try
      {
      valid_double = std::stod(input);
      if (valid_double < minval  || valid_double > maxval)
        {
        cout << "The entered value must be with the range " << minval << " to " << maxval << endl;
        }
      else
        {
        return valid_double;
        }

      }
    catch (...)
      {

      cout << "invalid value\n" << endl;
      }
    }
  }



/* use cout to send the first parameter "text" to the console
   take in input from the user and store it in the second parameter */

template <class T>
void Prompt (string prompt, T & val)
  {

  cout << prompt;
  cin >> val;
  while(cin.fail())
    {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout <<  prompt;
    cin >> val;
    }
  }


/*
   bool PromptYN(string input)
   display the string 'input'
   give the user a prompt
   if they type Y or y return true
   if they type N or n return false
   if they type anything else
   keep repeating till they type Y,y,N or n

usage:
if ( PromptYN("Do you like SPAM & eggs?")
cout << "you answered Y or y" << endl;
else
cout << "you answered N or n" << endl;
 */


inline bool PromptYN(string input)
  {
  while (true)
    {
    cout << input << " (Y/N)"<< endl;
    ignore_line( cin ); // clear the buffer if needed
    string user_input;
    cin >> user_input;
    if ( user_input == "Y" || user_input =="y")
      {
      return true;
      }
    if (user_input == "N" || user_input =="n")
      {
      return false;
      }
    }
  }






/*********************************************
  system stuff
 *********************************************/

// Do nothing for a period of time
// // used to slow the operation of programs
// // the parameter passed in will be approximatley in 100ths of a second
// // 25 -> quarter of a second, 50 -> half a second
inline void WaitHundredth(int time)
  {
  struct timespec tim, tim2;
  tim.tv_sec = 0;
  tim.tv_nsec = time*10000000;
  nanosleep(&tim , &tim2);
  }


// // clear the screen
#define ClearScreen() system("clear")


/*
   GetTime()
   returns the current time as a formatted char array
   The returned string has the following format:
   Www Mmm dd hh:mm:ss yyyy
   Where Www is the weekday, Mmm the month in letters, dd the day of the month, hh:mm:ss the time, and yyyy the year.
   The string is followed by a new-line character ('\n') and the terminating null-character.
 */
inline char * GetTime()
  {
  time_t rawtime = time(0);
  struct tm * timeinfo;

  timeinfo = localtime ( &rawtime );
  return asctime (timeinfo);
  }




/****************************************
  Numberic fuctions

 *****************************************/

/*
   this will try to convert a string to in integer
   may throw and exception if it cant do it
 */

inline int StringToInteger( string input)
  {
  return stoi(input);
  }

inline double  StringToDouble( string input )
  {
  return stod(input);
  }


inline bool StringToBool(string input)
  {
  if(input=="true" || input =="1")
    {
    return true;
    }

  if(input=="false" || input =="0")
    {
    return false;
    }

  return false;
  }



/*
   compare some floating point numbers
   this should work for type double as well

   it is difficult to compare for equality so this will tell us if they are within
   a distance of each other the default for that value is .0005

 */
inline bool VeryClose(float A, float B, float epsilon = 0.0005f)
  {
  return (fabs(A - B) < epsilon);
  }




/*
   this will try to convert a string to in integer
   may throw and exception if it cant do it
 */
template <typename T>
string NumberToString ( T Number )
  {
  ostringstream ss;
  ss << Number;
  return ss.str();
  }


inline int CreateRandomNumber(int min, int max)
  {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(min,max);
  return (dis(gen));

  /*
     if ( 1804289383 == rand())
     srand(time(NULL));
     return ( ( rand() % (max-min+1) ) + min);
   */

  }

inline bool IsNumber(string str)
  {
  for (unsigned int i = 0; i < str.length(); i++)
    {
    if (! isdigit(str[i]))
      {
      return false;
      }

    }
  return true;
  }

inline bool IsInteger(string input)
  {
  try
    {
    int temp = stoi(input);
    temp++; // to avoid unused var warning
    }
  catch(...)
    {
    return false;
    }
  return true;
  }


inline bool IsDouble(string input)
  {

  try
    {
    double temp = stod(input);
    temp++; // to avoid unused var warning
    }
  catch(...)
    {
    return false;
    }
  return true;
  }



inline string PassFail(bool in)
  {
  return (in) ? "Pass":"Fail";
  }
inline string PF(bool in)
  {
  return (in) ? "Pass":"Fail";
  }


/***************************************
  console functions
 ****************************************/


/*
   returns the input string wrapped in the commands to change color , display the text and change back to white

   cout << "the cat is " << RED("named felix" << " the cat" << endl;

   only "named felix" will be red
 */

inline string RED(string input)
  {
  return string("\033[1;31m"+input+"\033[0m");
  }

inline string GREEN(string input)
  {
  return string("\033[1;32m"+input+"\033[0m");
  }

inline string YELLOW(string input)
  {
  return string("\033[1;33m"+input+"\033[0m");
  }

inline string BLUE(string input)
  {
  return string("\033[1;34m"+input+"\033[0m");
  }

inline string MAGENTA(string input)
  {
  return string("\033[1;35m"+input+"\033[0m");
  }

inline string CYAN(string input)
  {
  return string("\033[1;36m"+input+"\033[0m");
  }




/* returns a string that when pushed to cout will change the console output color
   you will probably want to set it back white at some point */

inline string ChangeToRed()
  {
  return string("\033[1;31m");
  }
inline string ChangeToGreen()
  {
  return string("\033[1;32m");
  }
inline string ChangeToYellow()
  {
  return string("\033[1;33m");
  }
inline string ChangeToBlue()
  {
  return string("\033[1;34m");
  }
inline string ChangeToMagenta()
  {
  return string("\033[1;35m");
  }
inline string ChangeToCyan()
  {
  return string("\033[1;36m");
  }
inline string ChangeToWhite()
  {
  return string("\033[0m");
  }



/* move the cursor to the position indcated by the row and column.. Note the offset is 1 not 0 */
inline void PositionCursor(int row,int col)
  {
  printf("\033[%i;%iH",row+1,col+1);
  }



/*********************************************************************************************************************
 **********************************************************************************************************************

 Logging functions and macros

 you need to define LOGLEVEL   0-3
 LOGLEVEL not defined            all logging functions are left out of compiled code
 0 none                          all logging functions are left out of compiled code
 1                               low level LogLow function calls only
 2                               med level   LogLow, LogMed , LogEnd ,LogStart
 3                               high level  everything

 you can do it with something like
#define LOGLEVEL 2


you MUST have the following in any main.cpp (AND ONLY IN THE MAIN) where you want to use the logging

#include "logger.h"
struct LOGGER_DATA LGDATA; // create a global instance of this struct .. needed by logging library


 **********************************************************************************************************************
 **********************************************************************************************************************/

#if LOGLEVEL < 1   || (! defined(LOGLEVEL))
// we wil put in some stubs to make all the logging functions basically do nothing
#define LOGGER_DATA
#define LoggerSetup(...)
#define LoggerSetUpLog(...)    //do nothing
#define LogLow(...)    //do nothing
#define LogMed(...)    //do nothing
#define LogHigh(...)    //do nothing
#define LogPos() // do nothing
#define LogStart(...)
#define LogEnd(...)
#define LogEndReturning(...)
// end all the stubs
#endif  // LOGLEVEL not defined or 0




/* here we will put in the actual functions and working logging */
#if LOGLEVEL > 0



struct LOGGER_DATA
  {
  FILE * LF;
  int    INDENT_COUNT;
  char   INDENT_BUFFER[1000];
  char   FN[100];
  int    Frame;
  int initialized;
  LOGGER_DATA() {FN[0]=0;}
  };
typedef struct LOGGER_DATA LOGGER_DATA;

extern  LOGGER_DATA  LGDATA;



#define GET_MACRO(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME, ...) NAME

#define LogStart(...) GET_MACRO(_0, ##__VA_ARGS__, LogStart10, LogStart9, LogStart8, LogStart7, LogStart6, LogStart5, LogStart4, LogStart3, LogStart2, LogStart1, LogStart0)(__VA_ARGS__)
#define LogStart0()                       LoggerStart(__PRETTY_FUNCTION__,__LINE__);
#define LogStart1(a)                      LoggerStart(__PRETTY_FUNCTION__,__LINE__,AsString(a));
#define LogStart2(a,b)                    LoggerStart(__PRETTY_FUNCTION__,__LINE__,AsString(a),AsString(b));
#define LogStart3(a,b,c)                  LoggerStart(__PRETTY_FUNCTION__,__LINE__,AsString(a),AsString(b),AsString(c));
#define LogStart4(a,b,c,d)                LoggerStart(__PRETTY_FUNCTION__,__LINE__,AsString(a),AsString(b),AsString(c),AsString(d));
#define LogStart5(a,b,c,d,e)              LoggerStart(__PRETTY_FUNCTION__,__LINE__,AsString(a),AsString(b),AsString(c),AsString(d),AsString(e));
#define LogStart6(a,b,c,d,e,f)            LoggerStart(__PRETTY_FUNCTION__,__LINE__,AsString(a),AsString(b),AsString(c),AsString(d),AsString(e),AsString(f));
#define LogStart7(a,b,c,d,e,f,g)          LoggerStart(__PRETTY_FUNCTION__,__LINE__,AsString(a),AsString(b),AsString(c),AsString(d),AsString(e),AsString(f),AsString(g));
#define LogStart8(a,b,c,d,e,f,g,h)        LoggerStart(__PRETTY_FUNCTION__,__LINE__,AsString(a),AsString(b),AsString(c),AsString(d),AsString(e),AsString(f),AsString(g),AsString(h));
#define LogStart9(a,b,c,d,e,f,g,h,i)      LoggerStart(__PRETTY_FUNCTION__,__LINE__,AsString(a),AsString(b),AsString(c),AsString(d),AsString(e),AsString(f),AsString(g),AsString(h),AsString(i));
#define LogStart10(a,b,c,d,e,f,g,h,i,j)   LoggerStart(__PRETTY_FUNCTION__,__LINE__,AsString(a),AsString(b),AsString(c),AsString(d),AsString(e),AsString(f),AsString(g),AsString(h),AsString(i),AsString(j));

#define LogEnd(...) GET_MACRO(_0, ##__VA_ARGS__, LogEnd10, LogEnd9, LogEnd8, LogEnd7, LogEnd6, LogEnd5, LogEnd4, LogEnd3, LogEnd2, LogEnd1, LogEnd0)(__VA_ARGS__)
#define LogEnd0()                      LoggerEnd(__PRETTY_FUNCTION__,__LINE__);
#define LogEnd1(a)                     LoggerEnd(__PRETTY_FUNCTION__,__LINE__,AsString(a));
#define LogEnd2(a,b)                   LoggerEnd(__PRETTY_FUNCTION__,__LINE__,AsString(a),AsString(b));
#define LogEnd3(a,b,c)                 LoggerEnd(__PRETTY_FUNCTION__,__LINE__,AsString(a),AsString(b),AsString(c));
#define LogEnd4(a,b,c,d)               LoggerEnd(__PRETTY_FUNCTION__,__LINE__,AsString(a),AsString(b),AsString(c),AsString(d));
#define LogEnd5(a,b,c,d,e)             LoggerEnd(__PRETTY_FUNCTION__,__LINE__,AsString(a),AsString(b),AsString(c),AsString(d),AsString(e));
#define LogEnd6(a,b,c,d,e,f)           LoggerEnd(__PRETTY_FUNCTION__,__LINE__,AsString(a),AsString(b),AsString(c),AsString(d),AsString(e),AsString(f));
#define LogEnd7(a,b,c,d,e,f,g)         LoggerEnd(__PRETTY_FUNCTION__,__LINE__,AsString(a),AsString(b),AsString(c),AsString(d),AsString(e),AsString(f),AsString(g));
#define LogEnd8(a,b,c,d,e,f,g,h)       LoggerEnd(__PRETTY_FUNCTION__,__LINE__,AsString(a),AsString(b),AsString(c),AsString(d),AsString(e),AsString(f),AsString(g),AsString(h));
#define LogEnd9(a,b,c,d,e,f,g,h,i)     LoggerEnd(__PRETTY_FUNCTION__,__LINE__,AsString(a),AsString(b),AsString(c),AsString(d),AsString(e),AsString(f),AsString(g),AsString(h),AsString(i));
#define LogEnd10(a,b,c,d,e,f,g,h,i,j)  LoggerEnd(__PRETTY_FUNCTION__,__LINE__,AsString(a),AsString(b),AsString(c),AsString(d),AsString(e),AsString(f),AsString(g),AsString(h),AsString(i),AsString(j));

#define LogEndReturning(a)   LoggerEndReturning(__PRETTY_FUNCTION__,__LINE__,a)

#define LogPos() LoggerLow(__PRETTY_FUNCTION__,__LINE__,"")
#define LogLow(a, ...) LoggerLow(__PRETTY_FUNCTION__,__LINE__,a,##__VA_ARGS__)
#define LogMed(a, ...) LoggerMed(__PRETTY_FUNCTION__,__LINE__,a,##__VA_ARGS__)
#define LogHigh(a, ...) LoggerHigh(__PRETTY_FUNCTION__,__LINE__,a,##__VA_ARGS__)


inline void LoggerOpen()
  {
  if( LGDATA.LF)
    {
    return;
    }
  if (strlen(LGDATA.FN) ==0)
    {
    cerr << "\nYou do not seem to have set up the logging in your main properly, look at the examples\n no i dont have a log file name \n"; 
    exit(0);
    }
 
  LGDATA.LF =fopen(LGDATA.FN,"a+");
   if  (LGDATA.LF == NULL)
        {
        cerr << "Could not open the logfile \""<< LGDATA.FN << "\", you may not have write permssions to that file: " << LGDATA.FN << endl;
        exit(0);
        }
  }

inline void LoggerClose()
  {
  if(! LGDATA.LF)
    {
    return;
    }
  fclose     ( LGDATA.LF   );
  LGDATA.LF = NULL;
  }

inline void LoggerSetIndent(int in)
  {
  LGDATA.INDENT_COUNT+=in;

  if (LGDATA.INDENT_COUNT <0)
    {
    LGDATA.INDENT_COUNT=0;
    }

  if (LGDATA.INDENT_COUNT >999)
    {
    LGDATA.INDENT_COUNT=999;
    }

  memset(LGDATA.INDENT_BUFFER,' ',LGDATA.INDENT_COUNT);
  LGDATA.INDENT_BUFFER[LGDATA.INDENT_COUNT]=0;
  }

// varidac functions for the logging to log end of function

// WriteParams handle all the values for all the parameters for LogStart and LogEnd after the first one
//inline  void WriteParams() {}

//template <typename T, typename...  Args>
//void WriteParams(const T & val,const Args... args)
//  {
//  fprintf(LGDATA.LF,"%sparam: %s\n",LGDATA.INDENT_BUFFER,AsString(val).c_str());
//  WriteParams(args...);
//  }


inline void LoggerEnd(const char * fn,const int line ,string a="",string b="",string c="",string d="",string e="",string f="",string g="",string h="",string i="",string j="")
  {
  LoggerOpen();
  fprintf(LGDATA.LF,"%sEnd -- frame(%d) %s\n",LGDATA.INDENT_BUFFER,LGDATA.Frame,fn);

  if(a.length() >0)
    {
    fprintf(LGDATA.LF,"%sparam: %s\n",LGDATA.INDENT_BUFFER,a.c_str());
    }
  if(b.length() >0)
    {
    fprintf(LGDATA.LF,"%sparam: %s\n",LGDATA.INDENT_BUFFER,b.c_str());
    }
  if(c.length() >0)
    {
    fprintf(LGDATA.LF,"%sparam: %s\n",LGDATA.INDENT_BUFFER,c.c_str());
    }
  if(d.length() >0)
    {
    fprintf(LGDATA.LF,"%sparam: %s\n",LGDATA.INDENT_BUFFER,d.c_str());
    }
  if(e.length() >0)
    {
    fprintf(LGDATA.LF,"%sparam: %s\n",LGDATA.INDENT_BUFFER,e.c_str());
    }
  if(f.length() >0)
    {
    fprintf(LGDATA.LF,"%sparam: %s\n",LGDATA.INDENT_BUFFER,f.c_str());
    }
  if(g.length() >0)
    {
    fprintf(LGDATA.LF,"%sparam: %s\n",LGDATA.INDENT_BUFFER,g.c_str());
    }
  if(h.length() >0)
    {
    fprintf(LGDATA.LF,"%sparam: %s\n",LGDATA.INDENT_BUFFER,h.c_str());
    }
  if(i.length() >0)
    {
    fprintf(LGDATA.LF,"%sparam: %s\n",LGDATA.INDENT_BUFFER,i.c_str());
    }
  if(j.length() >0)
    {
    fprintf(LGDATA.LF,"%sparam: %s\n",LGDATA.INDENT_BUFFER,j.c_str());
    }

  LoggerSetIndent(-2);

  if (0 == LGDATA.INDENT_COUNT)
    {
    fprintf(LGDATA.LF,"\n\n");
    }
  LoggerClose();
  LGDATA.Frame--;
  }

inline void LoggerStart(const char * fn,const int line,string a="",string b="",string c="",string d="",string e="",string f="",string g="",string h="",string i="",string j="")
  {
  LoggerOpen();
  LoggerSetIndent(2);

  LGDATA.Frame++;
  fprintf(LGDATA.LF,"%sStart -- frame(%d) %s\n",LGDATA.INDENT_BUFFER,LGDATA.Frame,fn);

  if(a.length() >0)
    {
    fprintf(LGDATA.LF,"%sparam: %s\n",LGDATA.INDENT_BUFFER,a.c_str());
    }
  if(b.length() >0)
    {
    fprintf(LGDATA.LF,"%sparam: %s\n",LGDATA.INDENT_BUFFER,b.c_str());
    }
  if(c.length() >0)
    {
    fprintf(LGDATA.LF,"%sparam: %s\n",LGDATA.INDENT_BUFFER,c.c_str());
    }
  if(d.length() >0)
    {
    fprintf(LGDATA.LF,"%sparam: %s\n",LGDATA.INDENT_BUFFER,d.c_str());
    }
  if(e.length() >0)
    {
    fprintf(LGDATA.LF,"%sparam: %s\n",LGDATA.INDENT_BUFFER,e.c_str());
    }
  if(f.length() >0)
    {
    fprintf(LGDATA.LF,"%sparam: %s\n",LGDATA.INDENT_BUFFER,f.c_str());
    }
  if(g.length() >0)
    {
    fprintf(LGDATA.LF,"%sparam: %s\n",LGDATA.INDENT_BUFFER,g.c_str());
    }
  if(h.length() >0)
    {
    fprintf(LGDATA.LF,"%sparam: %s\n",LGDATA.INDENT_BUFFER,h.c_str());
    }
  if(i.length() >0)
    {
    fprintf(LGDATA.LF,"%sparam: %s\n",LGDATA.INDENT_BUFFER,i.c_str());
    }
  if(j.length() >0)
    {
    fprintf(LGDATA.LF,"%sparam: %s\n",LGDATA.INDENT_BUFFER,j.c_str());
    }

  LoggerClose();
  }




inline void segfault_sigaction(int signal, siginfo_t *si, void *arg)
  {
  psignal(signal,"\n\nYour program experienced an error\nsignal:\n");
  psiginfo(si,"siginfo_t:\n");
  exit(0);
  }

inline void LoggerSetUpLog(const char * filename, bool append=false)
  {
  strncpy(LGDATA.FN,filename,100);
  LGDATA.LF=NULL;
  LGDATA.INDENT_COUNT=0;
  LGDATA.Frame=0;

  if (! append)
    {
    LGDATA.LF = fopen(LGDATA.FN,"w+");
   if  (LGDATA.LF == NULL)
        {
        cerr << "Could not open the logfile, you may not have write permssions to the file: " << LGDATA.FN << endl;
        exit(0);
        }


    }
  else
    {
    LoggerOpen();
    }

  fprintf(LGDATA.LF,"log file \"%s\" opened\n",LGDATA.FN );
  LoggerClose();

  struct sigaction sa;
  int size = sizeof(sa);
  memset(&sa, 0, size);
  sigemptyset(&sa.sa_mask);
  sa.sa_sigaction = segfault_sigaction;
  sa.sa_flags   = SA_SIGINFO;

  sigaction(SIGSEGV, &sa, NULL);
  }



template<class T>
void* GetAddress( T & in)
  {
  return & in;
  }

template<class T>
void* GetAddress( T * in)
  {
  return ((void*) in);
  }



template <typename T>
inline string ValueAsString(T & in)
  {

  int status=0;
  string name = abi::__cxa_demangle( typeid(T).name(),0,0,&status);


  ostringstream out;
  if (name=="int" )
    {
    out << * ((int *) &in );
    return out.str();
    }

  if (name=="unsigned long" )
    {
    out << * ((unsigned long *) &in );
    return out.str();
    }

  if (name=="long" )
    {
    out << * ((long *) &in );
    return out.str();
    }

  if (name=="long long" )
    {
    out << * ((long long *) &in );
    return out.str();
    }

  if (name=="unsigned long long" )
    {
    out << * ((unsigned long long *) &in );
    return out.str();
    }

  if (name=="short" )
    {
    out << * ((short *) &in );
    return out.str();
    }

  if (name=="unsigned short" )
    {
    out << * ((unsigned short *) &in );
    return out.str();
    }

  if (name=="double" )
    {
    out << * ((double  *) &in );
    return out.str();
    }

  if (name=="long double" )
    {
    out << * ((long double  *) &in );
    return out.str();
    }

  if (name=="char" )
    {
    out << * ((char  *) &in );
    return out.str();
    }

  if (name=="char *" )
    {
    out << * ((char  *) &in );
    return out.str();
    }

  if (name=="char const*" )
    {
    out << * ((char const *) &in );
    return out.str();
    }

  if (name=="unsigned char" )
    {
    out << * ((unsigned char  *) &in );
    return out.str();
    }

  if (name=="bool" )
    {
    out << boolalpha << * ((bool  *) &in );
    return out.str();
    }

  if (name=="std::string" )
    {
    out << * ((std::string  *) &in );
    return out.str();
    }

  if (name=="std::istream" )
    {
    out << "cant display value of std::istream" ;
    return out.str();
    }

  if (name=="std::ostream" )
    {
    out << "cant display value of std::ostream" ;
    return out.str();
    }

  if (name=="std::ifstream" )
    {
    out << "cant display value of std::ifstream" ;
    return out.str();
    }

  if (name=="std::ofstream" )
    {
    out << "cant display value of std::ofstream" ;
    return out.str();
    }

  return string("unknown type <" + name + ">, cant display value try converting it to a string or overoad ostream << \n");
  }
template <class T>
string GetClassName(T&input)
{

  int status=0;
  string name = abi::__cxa_demangle( typeid(T).name(),0,0,&status);
  return name;
} 

template <class T>
string GetClassName(T * input)
{

  int status=0;
  string name = abi::__cxa_demangle( typeid(T).name(),0,0,&status);
  name += " pointer"; 
  return name;
} 




template <class T>
string AsString(T  & input)
  {
  string name = GetClassName(input);
  ostringstream out;
  out  << name<< " address:" << ((void*) & input) << " value:" << ValueAsString(input);
  return out.str();
  }

template <class T>
string AsString(T *input)
  {
  string name = GetClassName(input);
  ostringstream out;
  if(input == nullptr)
    {
    out << name <<  "*  address:" << ((void *)input) << " points_to_value:nullptr";
    }
  else
    {

    if (name=="char"  || name == "char const")
      {
      out << name <<  "*  address:" << ((void *)input) << " points_to_value:"<< input ;
      }
    else
      {
      out << name <<  "*  address:" << ((void *)input) << " points_to_value:"<< ValueAsString(*input);
      }

    }
  return out.str();
  }

inline string AsString()
  {
  return string("");
  }

// WriteFormatted do the work for LogLow, LogMed, LogHigh

inline void WriteFormmatted(const char* format)
  {
  fprintf(LGDATA.LF,"%s",format);
  }

template<typename T, typename... Args>
static void WriteFormmatted(const char* format, T  value, Args...  args)
  {
  while (format && *format)
    {
    if (*format=='%' && *++format!='%')
      {
      if (*format == 'p')
        {
        fprintf(LGDATA.LF,"%p",GetAddress(value));  // log just the address
        }
      else if (*format == 'S')
        {
        fprintf(LGDATA.LF,"%s",AsString(value).c_str());  // show with the datatype name
        }
      else
        {
        fprintf(LGDATA.LF,"%s",ValueAsString(value).c_str());  // just the value
        }
        {
        return WriteFormmatted(++format, args...);  // ``peel off'' first argument
        }
      }
    fprintf(LGDATA.LF,"%c",*format++);
    }
  // if we are down here there are more variables passed in that corresponding place holders   %P %S %s
  fprintf(LGDATA.LF,"\n\nextra arguments provided to Logger::Log   you need a placeholder %%P or %%S or %%s for each additional parameter");
  }





template <typename... Args>
static void LoggerLow(const char * fn, int ln,const char* format, Args...  args)
  {
#if LOGLEVEL >2
  LoggerOpen();
  fprintf(LGDATA.LF,"%s%s at line %d  :",LGDATA.INDENT_BUFFER,fn,ln);// indent
  WriteFormmatted(format, args...);
  LoggerClose();
#endif
  }

template <typename... Args>
static void LoggerMed(const char * fn, int ln,const char* format, Args...  args)
  {
#if LOGLEVEL >1
  LoggerOpen();
  fprintf(LGDATA.LF,"%s%s at line %d  :",LGDATA.INDENT_BUFFER,fn,ln);// indent
  WriteFormmatted(format, args...);
  LoggerClose();
#endif
  }
template <typename... Args>
static void LoggerHigh(const char * fn, int ln, const char* format, Args...  args)
  {
#if LOGLEVEL >0
  LoggerOpen();
  fprintf(LGDATA.LF,"%s%s at line %d  :",LGDATA.INDENT_BUFFER,fn,ln);// indent
  WriteFormmatted(format, args...);
  LoggerClose();
#endif
  }


template <typename T>
void LoggerEndReturning(const char * fn,int ln,const T & val)
  {
  LoggerOpen();
  fprintf(LGDATA.LF,"%sEnd frame(%d)--%s at line %d\nReturn Value:%s",LGDATA.INDENT_BUFFER,LGDATA.Frame,fn,ln,ValueAsString(val).c_str());
  fprintf(LGDATA.LF,"\n\n");
  LoggerSetIndent(-2);
  LoggerClose();
  LGDATA.Frame--;
  }



#endif  // LOGLEVEL > 0 










/*********************************************************************************************************************
 **********************************************************************************************************************

 below are fucnctions that you wont be calling they are
 for use by other functions


 **********************************************************************************************************************
 **********************************************************************************************************************/

template <typename CharT>
std::streamsize ignore_line (
  std::basic_istream<CharT>& in, bool always_discard )
  {
  std::streamsize nread = 0;
  if ( always_discard
       || ( in.rdbuf()->sungetc() != std::char_traits<CharT>::eof()
            && in.get() != in.widen ( '\n' ) ) )
    {
    // The stream is good, and we haven't
    // read a full line yet, so clear it out
    in.ignore ( std::numeric_limits<std::streamsize>::max(), in.widen ( '\n' ) );
    nread = in.gcount();
    }
  return nread;
  }


