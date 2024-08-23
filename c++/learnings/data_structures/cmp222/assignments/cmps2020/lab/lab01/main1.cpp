#include "cmpslib.h" // all the special functions provided for this class

struct LOGGER_DATA LGDATA; // create a global instance of this struct .. needed by logging library




// create function     int TimesThree(int)
int TimesThree(int val)
{  
    LogStart(val);
    LogEndReturning(val*3);
      return(val *3);
}
// returns value passed in multiplied by 3



// create function string OddOrEven(int)
string OddOrEven(int val)
{
    if(val%2 ==0)
    {
        return "Even";
    }
    else{
        return "False";
    }

}

// returns "Odd" or "Even" 



int main()
{
	LoggerSetUpLog("main1.log");
   LogLow("Starting a loop of 10 iterations");
   for(int i=0; i <10; i++)
   {
         LogLow("Inside The Loop With a Value of %i \n",i);
         TimesThree(i);

   }

	return 0;
}





