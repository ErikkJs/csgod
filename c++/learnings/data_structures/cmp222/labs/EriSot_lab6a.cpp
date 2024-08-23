// erik soto
// march 3 2016
// lab 6


#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>

using namespace std;

class ConfigFileError
{ 
    private:
        string _error;
    public:

        // overlaoded and regular constructor
        ConfigFileError()
        {
            _error = "ERROR.";
        }

        ConfigFileError(string t)
        {
            _error = t;
        }

        string get_error()
        {
            return _error;
        }

};

class ConfigFileBadKey : public ConfigFileError
{
    public:

        ConfigFileBadKey(): ConfigFileError("Bad Key")
    {}
};

class ConfigFileMissingKey : public ConfigFileError
{public:

    ConfigFileMissingKey() : ConfigFileError("MISSING KEY")
    {}
};

class ConfigFileMissingSeperator : public ConfigFileError
{
    public: 
        ConfigFileMissingSeperator() : ConfigFileError("MISSING SEPERATOR")
    {}

};


class ConfigFile
{
    private:
        // looks for seperator
        bool contains_separator(string line , char eq ='=')
        {
            int equalcount =0;

            for(int i =0; i<line.length(); i++)
            {
                if( line[i] == eq)
                {
                    equalcount++;
                }
            }

            if(equalcount ==1)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        // CHECKS FOR IF NAME IS VALID
        bool is_name_valid(string a)
        {

            if( isalpha((a[0])) && islower(a[0]))
            {
                return true;
            }
            else
            {
                return false;
            }

        }


            // checks to see if comment
        bool is_comment(string b)
        {
            if(b[0] == ';')
            {
                return true;
            }
            else
            {
                return false;
            }


        }
        // checks for a missing key
        bool missingkey( string line)
        {
            int dex =0;
            bool flag = false;

            for(int i =0; i<line.length(); i++)
            {
                if(line[i] == '=')
                { 
                    dex = i+1;
                }
            }

            if(line[dex] != '\0')
            {
                flag =true;
            }
            else 
                flag = false;
            return flag;
        }




    public:
        bool load(char * a)
        {
            string line;
            bool success = false;
           
           // loads file line by line, checks for errors through if statements
            ifstream ifs(a);
            if(ifs.good())
            {
                while(!ifs.eof())
                {
                    getline(ifs,line);
                    if(ifs.good())
                    {  
                        if(!is_comment(a));
                        {

                            if(missingkey(a))
                            {
                                success = true;
                            }
                            else
                            {
                                success =false;
                                throw ConfigFileMissingKey();
                                break;
                            }
                            if(contains_separator(a))
                            {
                                success =true;
                            }
                            else 
                            {
                                throw ConfigFileMissingSeperator();
                                break;
                            }
                            if(is_name_valid(a))
                            {
                                success = true;
                            }
                            else
                            {
                                success = false;
                                throw ConfigFileBadKey();
                                break;
                            }

                        }
                    }
                }
                ifs.close();

            }

            return success;
        }
};


int main()
{


    try
    {
        ConfigFile c;
        c.load("config.ini");

     
    catch(ConfigFileErrorBadKey & c)
    {
        cerr << c.get_error() << endl;
    }
    catch(ConfigFileMissingKey & c)
    {
        cerr << c.get_error() << endl;
    }

    catch(ConfigFileMissingSeperator & c)
    {
        cerr << c.get_error() << endl;
    }
    };
    return 0;

}

