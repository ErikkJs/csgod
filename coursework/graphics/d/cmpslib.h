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

using std::string;
using std::cout;
using std::cin;
using std::ostringstream;
using std::setfill;
using std::setw;
using std::endl;



// this one is used by the prompt functions you wont need it
template <typename CharT>
std::streamsize ignore_line (std::basic_istream<CharT>& in, bool always_discard = false );



/**************************************
	user input
 ***************************************/


/* use std::cout  to send the first parameter "text" to the console
	 take in input from the user and store it in the second parameter */



// a version specific for when parmam 2 is a string, read in a whole line
inline void Prompt (string prompt, string & val)
{
	ignore_line( std::cin );// clear the buffer if needed
	std::cout  << prompt;
	getline(std::cin,  val); // this 
}

// a generic verison for all other data types
	template <class T>
inline void Prompt (string prompt, T & val)
{

	std::cout  << prompt;
	std::cin >> val;
	while(std::cin.fail())  // if it fails... ie user input does not match the datatype
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout  <<  prompt;
		std::cin >> val;
	}
}


/* use std::cout  to send the first parameter "text" to the console
	 take in input from the user until they enter a valid integer
	 value between min and max , the 2nd and 3rd parameters */
	template <class T>
inline void Prompt( string prompt,T& val, T minval, T maxval)
{
	Prompt(prompt,val);
	while ( val < minval  || val > maxval)
	{
		std::cout  << "The entered value must be with the range " << minval << " to " << maxval << std::endl;
		Prompt(prompt,val);
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
std::cout  << "you answered Y or y" << std::endl;
else
std::cout  << "you answered N or n" << std::endl;
	 */


	inline bool PromptYN(string input)
	{
		while (true)
		{
			std::cout  << input << " (Y/N)"<< std::endl;
			ignore_line( std::cin ); // clear the buffer if needed
			string user_input;
			std::cin >> user_input;
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


	inline string GetDateTime()
	{
		time_t t = time(0);   // get time now
		struct tm * now = localtime( & t );
		ostringstream tm;
		tm << (now->tm_year + 1900) << '-' 
			<< setfill('0') << setw(2) <<(now->tm_mon + 1) << '-'
			<< setfill('0') << setw(2) << now->tm_mday << "-"
			<< setfill('0') << setw(2) << now->tm_hour << ":"
			<< setfill('0') << setw(2) << now->tm_min << ":"
			<< setfill('0') << setw(2) << now->tm_sec ;
		return tm.str();
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
	inline	string NumberToString ( T Number )
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

/* a function that will tell you if an array is sorted
   params : pointer to the array, int array size  */
template<class T>
inline bool IsSortedAscending(T* data,int size)
 {
    for(int loop=0;loop<size-1;loop++)
			if(data[loop]>data[loop+1])
				return false;
    return true;
 }

/* this is a kinda cool way to do it, pass in a reference to the array
   unfortunatly it isnt very compatable with most exisitng progams as 
	 it implememts some new features to c++ 
   but you dont have to pass in the size as a 2nd parameter        */
template<class T,size_t N>
inline bool IsSortedAscending(T(&data )[N])
 {
    // data is a reference to the array and N is the size of the array
    for(int loop=0;loop<N -1;loop++)
			if(data[loop]>data[loop+1])
				return false;
    return true;
 }
  
  
template<class T>
inline bool IsSortedDescending(T* data,int size)
 {
    for(int loop=0;loop<size-1;loop++)
			if(data[loop]<data[loop+1])
				return false;
    return true;
 }

/* this is a kinda cool way to do it, pass in a reference to the array
   unfortunatly it isnt very compatable with most exisitng progams as 
	 it implememts some new features to c++ 
   but you dont have to pass in the size as a 2nd parameter        */
template<class T,size_t N>
inline bool IsSortedDescending(T(&data )[N])
 {
    // data is a reference to the array and N is the size of the array
    for(int loop=0;loop<N -1;loop++)
			if(data[loop]<data[loop+1])
				return false;
    return true;
 }
  
  
	/***************************************
		console functions
	 ****************************************/


	/*
		 returns the input string wrapped in the commands to change color , display the text and change back to white
		 std::cout  << "the cat is " << RED("named felix" << " the cat" << std::endl;
		 only "named felix" will be red
	 */

	inline string RED(string input){return string("\033[1;31m"+input+"\033[0m");}
	inline string GREEN(string input){return string("\033[1;32m"+input+"\033[0m");}
	inline string YELLOW(string input){return string("\033[1;33m"+input+"\033[0m");}
	inline string BLUE(string input){return string("\033[1;34m"+input+"\033[0m");}
	inline string MAGENTA(string input){return string("\033[1;35m"+input+"\033[0m");}
	inline string CYAN(string input){return string("\033[1;36m"+input+"\033[0m");}

	/* returns a string that when pushed to std::cout  will change the console output color
		 you will probably want to set it back white at some point */

	inline string ChangeToRed(){return string("\033[1;31m");}
	inline string ChangeToGreen(){return string("\033[1;32m");}
	inline string ChangeToYellow(){return string("\033[1;33m");}
	inline string ChangeToBlue(){return string("\033[1;34m");}
	inline string ChangeToMagenta(){return string("\033[1;35m");}
	inline string ChangeToCyan(){return string("\033[1;36m");}
	inline string ChangeToWhite(){return string("\033[0m");}



	/* move the cursor to the position indcated by the row and column.. Note the offset is 1 not 0 */
	inline void PositionCursor(int row,int col){printf("\033[%i;%iH",row+1,col+1);}



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

#include "cmpslib.h"
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
		char   FN[100];
		int	   initialized;
		int    i_temp;// we will use this for stuff again and again
		char * cp_temp;// we will use this over and over
		LOGGER_DATA() {FN[0]=0;}
	};

	typedef struct LOGGER_DATA LOGGER_DATA;

	extern  LOGGER_DATA  LGDATA;


	inline void LoggerOpen()
	{
		if( LGDATA.LF)
			return;
		if (strlen(LGDATA.FN) ==0)
		{
			std::cerr << "\nYou do not seem to have set up the logging in your main properly, look at the examples\n  i dont have a log file name \n"; 
			exit(0);
		}
		LGDATA.LF =fopen(LGDATA.FN,"a+");
		if  (LGDATA.LF == NULL)
		{
			std::cerr << "Could not open the logfile \""<< LGDATA.FN << "\", you may not have write permssions to that file: either delete it or change the permissons " << LGDATA.FN << std::endl;
			exit(0);
		}
	}

	inline void LoggerClose()
	{
		if(! LGDATA.LF)
			return;
		fclose     ( LGDATA.LF   );
		LGDATA.LF = NULL;
	}

  inline void IndentLogLine()
  {
  for (LGDATA.i_temp=0;LGDATA.i_temp<LGDATA.INDENT_COUNT;LGDATA.i_temp++)
		fprintf(LGDATA.LF," ");
  }

	inline void LoggerSetIndent(int in)
	{
		LGDATA.INDENT_COUNT+=in;
		if (LGDATA.INDENT_COUNT <0) LGDATA.INDENT_COUNT=0;
		if (LGDATA.INDENT_COUNT >999) LGDATA.INDENT_COUNT=999;
  	  fprintf(LGDATA.LF,"\n"); // basically we call this at start and end of the log for a function , so add a new line
	}


	inline void LoggerSetUpLog(const char * filename, bool append=false)
	{
		strncpy(LGDATA.FN,filename,100);
		LGDATA.LF=NULL;
		LGDATA.INDENT_COUNT=0;

		if (! append)
		{
			LGDATA.LF = fopen(LGDATA.FN,"w+");
			if  (LGDATA.LF == NULL)
			{
				std::cerr << "Could not open the logfile, you may not have write permssions to the file, try deleting it: " << LGDATA.FN << std::endl;
				exit(0);
			}

		}
		else
		{
			LoggerOpen();
		}

		LoggerClose();
	}



inline void LogValueWithType(){}
inline void LogValueWithType(const short & a)				{IndentLogLine();fprintf(LGDATA.LF,"(short) %d\n",a);}
inline void LogValueWithType(const unsigned short & a) 		{IndentLogLine();fprintf(LGDATA.LF,"(unsigned short) %d\n",a);}
inline void LogValueWithType(const unsigned int & a) 		{IndentLogLine();fprintf(LGDATA.LF,"(unsigned int) %d\n",a);}
inline void LogValueWithType(const int & a) 				{IndentLogLine();fprintf(LGDATA.LF,"(int) %d\n",a);}
inline void LogValueWithType(const long & a) 				{IndentLogLine();fprintf(LGDATA.LF,"(long) %ld\n",a);}
inline void LogValueWithType(const unsigned long & a) 		{IndentLogLine();fprintf(LGDATA.LF,"(unsigned long) %ld\n",a);}
inline void LogValueWithType(const long long & a) 			{IndentLogLine();fprintf(LGDATA.LF,"(long long) %lld\n",a);}
inline void LogValueWithType(const unsigned long long & a) 	{IndentLogLine();fprintf(LGDATA.LF,"(unsigned long long) %llu\n",a);}
inline void LogValueWithType(const char & a) 				{IndentLogLine();fprintf(LGDATA.LF,"(char) %c\n",a);}
inline void LogValueWithType(const unsigned char & a) 		{IndentLogLine();fprintf(LGDATA.LF,"(unsigned char) %c\n",a);}
inline void LogValueWithType(const char* & a) 				{IndentLogLine();fprintf(LGDATA.LF,"(char*) %s\n",a);}
inline void LogValueWithType(const std::string & a) 		{IndentLogLine();fprintf(LGDATA.LF,"(std::string) %s\n",a.c_str());}
inline void LogValueWithType(const bool & a) 				{IndentLogLine();fprintf(LGDATA.LF,"(bool) %d\n",a);}
inline void LogValueWithType(const double & a) 				{IndentLogLine();fprintf(LGDATA.LF,"(double) %f\n",a);}
inline void LogValueWithType(const long double & a) 		{IndentLogLine();fprintf(LGDATA.LF,"(long double) %Le\n",a);}
template <typename T> 
inline void LogValueWithType(const T & a) 
{
	IndentLogLine();
	fprintf(LGDATA.LF,"(");
	LGDATA.cp_temp = abi::__cxa_demangle( typeid(T).name(),0,0,&LGDATA.i_temp);
	fprintf(LGDATA.LF,"%s ",LGDATA.cp_temp);
	free (LGDATA.cp_temp);
	fprintf(LGDATA.LF,") cant safely display value \n");
}


inline void LogParam(){}
inline void LogParam(const short & a) 				{IndentLogLine();fprintf(LGDATA.LF,"param: (short) %d\n",a);}
inline void LogParam(const unsigned short & a) 		{IndentLogLine();fprintf(LGDATA.LF,"param: (unsigned short) %d\n",a);}
inline void LogParam(const unsigned int & a) 		{IndentLogLine();fprintf(LGDATA.LF,"param: (unsigned int) %d\n",a);}
inline void LogParam(const int & a) 				{IndentLogLine();fprintf(LGDATA.LF,"param: (int) %d\n",a);}
inline void LogParam(const long & a) 				{IndentLogLine();fprintf(LGDATA.LF,"param: (long) %ld\n",a);}
inline void LogParam(const unsigned long & a) 		{IndentLogLine();fprintf(LGDATA.LF,"param: (unsigned long) %ld\n",a);}
inline void LogParam(const long long & a) 			{IndentLogLine();fprintf(LGDATA.LF,"param: (long long) %lld\n",a);}
inline void LogParam(const unsigned long long & a) 	{IndentLogLine();fprintf(LGDATA.LF,"param: (unsigned long long) %llu\n",a);}
inline void LogParam(const char & a) 				{IndentLogLine();fprintf(LGDATA.LF,"param: (char) %c\n",a);}
inline void LogParam(const unsigned char & a) 		{IndentLogLine();fprintf(LGDATA.LF,"param: (unsigned char) %c\n",a);}
inline void LogParam(const char* & a) 				{IndentLogLine();fprintf(LGDATA.LF,"param: (char*) %s\n",a);}
inline void LogParam(const std::string & a) 		{IndentLogLine();fprintf(LGDATA.LF,"param: (std::string) %s\n",a.c_str());}
inline void LogParam(const bool & a) 				{IndentLogLine();fprintf(LGDATA.LF,"param: (bool) %d\n",a);}
inline void LogParam(const double & a) 				{IndentLogLine();fprintf(LGDATA.LF,"param: (double) %f\n",a);}
inline void LogParam(const long double & a) 		{IndentLogLine();fprintf(LGDATA.LF,"param: (long double) %Le\n",a);}
template <typename T> 
inline void LogParam(const T & a) 
{
	IndentLogLine();
	fprintf(LGDATA.LF,"param: (");
	LGDATA.cp_temp = abi::__cxa_demangle( typeid(T).name(),0,0,&LGDATA.i_temp);
	fprintf(LGDATA.LF,"%s ",LGDATA.cp_temp);
	free (LGDATA.cp_temp);
	fprintf(LGDATA.LF,") cant safely display value \n");
}


inline void LogValue(){}
inline void LogValue(const short & a) 				{IndentLogLine();fprintf(LGDATA.LF,"%d",a);}
inline void LogValue(const unsigned short & a) 		{IndentLogLine();fprintf(LGDATA.LF,"%d",a);}
inline void LogValue(const unsigned int & a) 		{IndentLogLine();fprintf(LGDATA.LF,"%d",a);}
inline void LogValue(const int & a) 				{IndentLogLine();fprintf(LGDATA.LF,"%d",a);}
inline void LogValue(const long & a) 				{IndentLogLine();fprintf(LGDATA.LF,"%ld",a);}
inline void LogValue(const unsigned long & a) 		{IndentLogLine();fprintf(LGDATA.LF,"%ld",a);}
inline void LogValue(const long long & a) 			{IndentLogLine();fprintf(LGDATA.LF,"%lld",a);}
inline void LogValue(const unsigned long long & a) 	{IndentLogLine();fprintf(LGDATA.LF,"%llu",a);}
inline void LogValue(const char & a) 				{IndentLogLine();fprintf(LGDATA.LF,"%c",a);}
inline void LogValue(const unsigned char & a) 		{IndentLogLine();fprintf(LGDATA.LF,"%c",a);}
inline void LogValue(const char* & a) 				{IndentLogLine();fprintf(LGDATA.LF,"%s",a);}
inline void LogValue(const std::string & a) 		{IndentLogLine();fprintf(LGDATA.LF,"%s",a.c_str());}
inline void LogValue(const bool & a) 				{IndentLogLine();fprintf(LGDATA.LF,"%d",a);}
inline void LogValue(const double & a) 				{IndentLogLine();fprintf(LGDATA.LF,"%f",a);}
inline void LogValue(const long double & a) 		{IndentLogLine();fprintf(LGDATA.LF,"%Le",a);}
template <typename T> 
inline void LogValue(const T & a) 
{
	IndentLogLine();
	fprintf(LGDATA.LF,"<cant safely display value>\n");
}






	// WriteFormatted do the work for LogLow, LogMed, LogHigh

	inline void WriteFormmatted(const char* format)
	{
		fprintf(LGDATA.LF,"%s",format);
	}

	//template<typename T, typename... Args>
	template<class  T, class... Args>
		static void WriteFormmatted(const char* format, T  value, Args...  args)
		{
			while (format && *format)
			{
				if (*format=='%' && *++format!='%')
				{
					if (*format == 'p')
					{
						fprintf(LGDATA.LF,"%p",((void*)&value));  // log just the address
					}
					else if (*format == 'S')
					{
						LogValueWithType(value);  // show with the datatype name
					}
					else
					{
						LogValue(value);  // just the value
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
			IndentLogLine();
			fprintf(LGDATA.LF,"%s at line %d  :",fn,ln);// indent
			WriteFormmatted(format, args...);
			LoggerClose();
		#endif
		}

	template <typename... Args>
		static void LoggerMed(const char * fn, int ln,const char* format, Args...  args)
		{
		#if LOGLEVEL >1
			LoggerOpen();
			IndentLogLine();
			fprintf(LGDATA.LF,"%s at line %d  :",fn,ln);// indent
			WriteFormmatted(format, args...);
			LoggerClose();
		#endif
		}
	template <typename... Args>
		static void LoggerHigh(const char * fn, int ln, const char* format, Args...  args)
		{
		#if LOGLEVEL >0
			LoggerOpen();
			IndentLogLine();
			fprintf(LGDATA.LF,"%s at line %d  :",fn,ln);// indent
			WriteFormmatted(format, args...);
			LoggerClose();
		#endif
		}




#define GET_MACRO(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME, ...) NAME

#define LogStart(...) GET_MACRO(_0, ##__VA_ARGS__, LogStart10, LogStart9, LogStart8, LogStart7, LogStart6, LogStart5, LogStart4, LogStart3, LogStart2, LogStart1, LogStart0)(__VA_ARGS__)
#define LogStart0()                       LoggerOpen();LoggerSetIndent(2);IndentLogLine();fprintf(LGDATA.LF,"Start--%s\n",__PRETTY_FUNCTION__);LoggerClose();
#define LogStart1(a)                      LoggerOpen();LoggerSetIndent(2);IndentLogLine();fprintf(LGDATA.LF,"Start--%s\n",__PRETTY_FUNCTION__);LogParam(a);LoggerClose();
#define LogStart2(a,b)                    LoggerOpen();LoggerSetIndent(2);IndentLogLine();fprintf(LGDATA.LF,"Start--%s\n",__PRETTY_FUNCTION__);LogParam(a);LogParam(b);LoggerClose();
#define LogStart3(a,b,c)                  LoggerOpen();LoggerSetIndent(2);IndentLogLine();fprintf(LGDATA.LF,"Start--%s\n",__PRETTY_FUNCTION__);LogParam(a);LogParam(b);LogParam(c);LoggerClose();
#define LogStart4(a,b,c,d)                LoggerOpen();LoggerSetIndent(2);IndentLogLine();fprintf(LGDATA.LF,"Start--%s\n",__PRETTY_FUNCTION__);LogParam(a);LogParam(b);LogParam(c);LogParam(d);LoggerClose();
#define LogStart5(a,b,c,d,e)              LoggerOpen();LoggerSetIndent(2);IndentLogLine();fprintf(LGDATA.LF,"Start--%s\n",__PRETTY_FUNCTION__);LogParam(a);LogParam(b);LogParam(c);LogParam(d);LogParam(e);LoggerClose();
#define LogStart6(a,b,c,d,e,f)            LoggerOpen();LoggerSetIndent(2);IndentLogLine();fprintf(LGDATA.LF,"Start--%s\n",__PRETTY_FUNCTION__);LogParam(a);LogParam(b);LogParam(c);LogParam(d);LogParam(e);LogParam(f);LoggerClose();
#define LogStart7(a,b,c,d,e,f,g)          LoggerOpen();LoggerSetIndent(2);IndentLogLine();fprintf(LGDATA.LF,"Start--%s\n",__PRETTY_FUNCTION__);LogParam(a);LogParam(b);LogParam(c);LogParam(d);LogParam(e);LogParam(f);LogParam(g);LoggerClose();
#define LogStart8(a,b,c,d,e,f,g,h)        LoggerOpen();LoggerSetIndent(2);IndentLogLine();fprintf(LGDATA.LF,"Start--%s\n",__PRETTY_FUNCTION__);LogParam(a);LogParam(b);LogParam(c);LogParam(d);LogParam(e);LogParam(f);LogParam(g);LogParam(h);LoggerClose();
#define LogStart9(a,b,c,d,e,f,g,h,i)      LoggerOpen();LoggerSetIndent(2);IndentLogLine();fprintf(LGDATA.LF,"Start--%s\n",__PRETTY_FUNCTION__);LogParam(a);LogParam(b);LogParam(c);LogParam(d);LogParam(e);LogParam(f);LogParam(g);LogParam(h);LogParam(i);LoggerClose();
#define LogStart10(a,b,c,d,e,f,g,h,i,j)   LoggerOpen();LoggerSetIndent(2);IndentLogLine();fprintf(LGDATA.LF,"Start--%s\n",__PRETTY_FUNCTION__);LogParam(a);LogParam(b);LogParam(c);LogParam(d);LogParam(e);LogParam(f);LogParam(g);LogParam(h);LogParam(i);LogParam(j);LoggerClose()



#define LogEnd(...) GET_MACRO(_0, ##__VA_ARGS__, LogEnd10, LogEnd9, LogEnd8, LogEnd7, LogEnd6, LogEnd5, LogEnd4, LogEnd3, LogEnd2, LogEnd1, LogEnd0)(__VA_ARGS__)
#define LogEnd0()                       LoggerOpen();IndentLogLine();fprintf(LGDATA.LF,"End--%s\n",__PRETTY_FUNCTION__);LoggerSetIndent(-2);LoggerClose();
#define LogEnd1(a)                      LoggerOpen();IndentLogLine();fprintf(LGDATA.LF,"End--%s\n",__PRETTY_FUNCTION__);LogParam(a);LoggerSetIndent(-2);LoggerClose();
#define LogEnd2(a,b)                    LoggerOpen();IndentLogLine();fprintf(LGDATA.LF,"End--%s\n",__PRETTY_FUNCTION__);LogParam(a);LogParam(b);LoggerSetIndent(-2);LoggerClose();
#define LogEnd3(a,b,c)                  LoggerOpen();IndentLogLine();fprintf(LGDATA.LF,"End--%s\n",__PRETTY_FUNCTION__);LogParam(a);LogParam(b);LogParam(c);LoggerSetIndent(-2);LoggerClose();
#define LogEnd4(a,b,c,d)                LoggerOpen();IndentLogLine();fprintf(LGDATA.LF,"End--%s\n",__PRETTY_FUNCTION__);LogParam(a);LogParam(b);LogParam(c);LogParam(d);LoggerSetIndent(-2);LoggerClose();
#define LogEnd5(a,b,c,d,e)              LoggerOpen();IndentLogLine();fprintf(LGDATA.LF,"End--%s\n",__PRETTY_FUNCTION__);LogParam(a);LogParam(b);LogParam(c);LogParam(d);LogParam(e);LoggerSetIndent(-2);LoggerClose();
#define LogEnd6(a,b,c,d,e,f)            LoggerOpen();IndentLogLine();fprintf(LGDATA.LF,"End--%s\n",__PRETTY_FUNCTION__);LogParam(a);LogParam(b);LogParam(c);LogParam(d);LogParam(e);LogParam(f);LoggerSetIndent(-2);LoggerClose();
#define LogEnd7(a,b,c,d,e,f,g)          LoggerOpen();IndentLogLine();fprintf(LGDATA.LF,"End--%s\n",__PRETTY_FUNCTION__);LogParam(a);LogParam(b);LogParam(c);LogParam(d);LogParam(e);LogParam(f);LogParam(g);LoggerSetIndent(-2);LoggerClose();
#define LogEnd8(a,b,c,d,e,f,g,h)        LoggerOpen();IndentLogLine();fprintf(LGDATA.LF,"End--%s\n",__PRETTY_FUNCTION__);LogParam(a);LogParam(b);LogParam(c);LogParam(d);LogParam(e);LogParam(f);LogParam(g);LogParam(h);LoggerSetIndent(-2);LoggerClose();
#define LogEnd9(a,b,c,d,e,f,g,h,i)      LoggerOpen();IndentLogLine();fprintf(LGDATA.LF,"End--%s\n",__PRETTY_FUNCTION__);LogParam(a);LogParam(b);LogParam(c);LogParam(d);LogParam(e);LogParam(f);LogParam(g);LogParam(h);LogParam(i);LoggerSetIndent(-2);LoggerClose();
#define LogEnd10(a,b,c,d,e,f,g,h,i,j)   LoggerOpen();IndentLogLine();fprintf(LGDATA.LF,"End--%s\n",__PRETTY_FUNCTION__);LogParam(a);LogParam(b);LogParam(c);LogParam(d);LogParam(e);LogParam(f);LogParam(g);LogParam(h);LogParam(i);LogParam(j);LoggerSetIndent(-2);LoggerClose();

#define LogEndReturning(a)   LoggerOpen();IndentLogLine();fprintf(LGDATA.LF,"End--%s  Returning\n",__PRETTY_FUNCTION__);LogValueWithType(a); fprintf(LGDATA.LF,"\n");LoggerSetIndent(-2);LoggerClose();

	//LoggerEndReturning(__PRETTY_FUNCTION__,__LINE__,a)

#define LogPos() LoggerLow(__PRETTY_FUNCTION__,__LINE__,"")
#define LogLow(a, ...) LoggerLow(__PRETTY_FUNCTION__,__LINE__,a,##__VA_ARGS__)
#define LogMed(a, ...) LoggerMed(__PRETTY_FUNCTION__,__LINE__,a,##__VA_ARGS__)
#define LogHigh(a, ...) LoggerHigh(__PRETTY_FUNCTION__,__LINE__,a,##__VA_ARGS__)


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


