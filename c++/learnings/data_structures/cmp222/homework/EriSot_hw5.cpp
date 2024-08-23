// erik soto
// march 3 2016
// lab 6


#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include <string>

using namespace std;

bool split( string key, string &value)
{
    int equals = line.find('=');

    key= line.substr(0, equals);

    value = line.substr(equals +1, line.length() - equals +1);

    return true;

}


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

class ConfigFileNonBooleanValue: public ConfigFileError
{
    public:
        ConfiFileNonBooleanValue() : ConfigFileError("NON Boolean")
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
            {4
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
}


bool get_value(string keyname, string & param_value)
{
    bool found = false;

    string line;
    string val;
    string key;

    ifstream ifs("config.ini");

    if(ifs.good())
    {
        while(!ifs.eof() && !found)
        {
            getline(ifs,line);
            if(ifs.good())
            {  
                split(line,key,val)
                    if(key==keyname)
                    {
                        param_value = value;
                        found = true;
                    }
            }
        }
    }        

    ifs.close();

    return found;
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
// these throw exceptions
string get_string( string key, string def_value)
{
    if(get_value(string key & value))
    {
        return value;
    }
    else 
    {

        return  def_value;
    }

}

// looks for the bool and acts accordingly
bool get_bool( string key , bool def_value)
{
    string value;
    if(get_value(key , value))
    { 

        bool found = true;

        for( int i = 0; i < value.length(); i++)
        {
            value[i] = toupper(value[i]);
        }



    }

}


// looks to see if its a value
double get_double( string key , double def_value)
{   
     string value;
    if(get_value( key, value))
    { 

        return  stod(value);
    }
    else
    {
        return def_value;
    }


}

// checks if its a int 
int get_int(string key , int def_value)
{
    string value;
    if(get_value(string key,value))
    {
        return stoi(value)
    }
    else 
    {
        return def_value;
    }


}

// overloads without default value parameters
// this can throw exception ConfigFileNonRealValue
double get_double(string key)
{
    string value;
    if(get_value( key, value))
    {
        return stod(value);

    }
    else ConfigFileNonRealValue;
}

// this can throw exception ConfigFileNonIntergerValue
int get_int(string key)
{
    string value;
    if(get_value(string key, value))
    {
        return stoi(key);
    }
    else
    {
        throw ConfigFileNonIntergerValue;
    }      

}

// this can throw exception ConfigFileNonBooleanValue
bool get_bool(string key)
{

    else
    {
        throw ConfigFileNonBooleanValue;
    }


}



int main()
{

    ConfigFile f;  
    try
    {   
        if (f.load("config.ini")) 
        {              
            cout << f.get_string("Name", "John") << endl;     
            cout << f.get_int("Year", 2016) << endl;       
            cout << f.get_int("Year") << endl;      
            if (f.get_bool("SaveOnExit", false))        
                cout << "SaveOnExit is true" << endl;    
            else       
                cout << "SaveOnExit is false" << endl;    
        }  
    } 
    catch(ConfigFileError &ex)   
    {
        cerr << ex.get_error() << endl;  
    }


    return 0;

}

