
#include "cmpslib.h" // all the special functions provided for this class

struct LOGGER_DATA LGDATA; // create a global instance of this struct .. needed by logging library




// put your funtions here

int SmallerInt(int & val1, int & val2)
{
    LogStart(val1,val2);
    if(val1 < val2)
    {
        LogEndReturning(val1);
        return val1;
    }else
        LogEndReturning(val2);
    return val2;


}

//int LargerInt(int & val, int & val2)
//{
//}
//int SwapInt(int & val, int & val2)
//{
//}

void TripleInt(int & val)
{
	LogStart(val);
	val *=3;
	LogEnd(val);
}




int main()
{
	LoggerSetUpLog("main1.log");

	cout << endl << endl;
	cout <<"Testing TripleInt" << endl;
	int temp=1;
	TripleInt(temp);
	cout << PassFail(3 == temp) << endl;

	TripleInt(temp);
	cout << PassFail(9 == temp) << endl;

	TripleInt(temp);
	cout << PassFail(27 == temp) << endl;

	TripleInt(temp);
	cout << PassFail(81 == temp) << endl;
   
   int int1=5;
   int int2=6;
   int smaller = SmallerInt(int1,int2);
   cout << PassFail(5==smaller) << endl;
   cout << PassFail(6==smaller) << endl;

	// continue testing the rest of your functions

	


	return 0;
}
