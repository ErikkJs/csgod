#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

struct QuakeData{
    double latitude;
    double longitude;
    double depth;
    double magnitude;

    string location;
    string id;

};
class Earthquake{
    protected:

        QuakeData Data[100];
        int records =0;

        // gets the strongest earthquake
        QuakeData get_strongest()
        {
            QuakeData test;
            test.magnitude =0;

            for(int i =0; i< records; i++)
            {

                if(Data[i].magnitude > test.magnitude)
                {
                    test.magnitude = Data[i].magnitude;
                    test.latitude = Data[i].latitude;
                    test.longitude = Data[i].longitude;
                    test.depth = Data[i].depth;
                    test.location = Data[i].location;
                    test.id = Data[i].id;

                }

            }
            return test;
        }



    public:
        // loads the array with information
        void load(const char * file)
        { 
            ifstream ifs(file);
            if (ifs.is_open())
            {
                string line;
                getline(ifs , line);
                int i= 0;
                while(!ifs.eof())
                {
                    QuakeData data;
                    double  la;
                    double lon;
                    double d;
                    double m;
                    string  loc;
                    string  id;

                    // reads the stuff and assigns it too stuff
                    ifs >> la >> lon >> d >> m >> loc >> id;

                    // checks  
                    if(ifs.good())
                    {
                        data.latitude= la;
                        data.longitude = lon;
                        data.depth = d;
                        data.magnitude = m;
                        data.location = loc;
                        data.id = id;

                        i++;
                        records++;

                        Data[i-1] = data;


                    }
                }
                ifs.close();
            }

        }

        virtual  bool save (char *) =0 ;
};

class CommaStream: public Earthquake{
    public:
        bool save(char * file) // save the data to file
        {
            // opens and writes a file seperated by commas
            ofstream fout(file);
            if(fout.is_open())
            {
                for(int i=0; i <records; i++)
                {
                    fout << Data[i].latitude << "," << Data[i].longitude << "," << Data[i].depth << "," << Data[i].magnitude << "," << Data[i].location << "," << Data[i].id << endl;
                } 

            }
            fout.close();

        }
};

class XMLStream : public Earthquake{
    public:

        // makes and saves a file in XML formaat

        bool save(char * file  )
        {
            QuakeData q;
            q = get_strongest();

            ofstream fout(file);
            if (fout.is_open())
            {
                fout <<  "?xml version= \"1.0\" encoding=  \" UTF-8\" ?>" << endl;
                fout <<  "<earthquake>" << endl;  
                fout <<  "  <events count= \"1\" desc= \"largest>\" >" << endl;
                fout <<  "    <event>" << endl;
                fout <<  "      <latitude>" << q.latitude << "</latitude>"<< endl;      
                fout <<  "      <longitude> " << q.longitude << "</longitude>" << endl;
                fout <<  "      <depth> " << q.depth << " </depth>" << endl;
                fout <<  "      <mag>" << q.magnitude << " </mag>"<< endl;
                fout <<  "      <location>" << q.location <<"</location>"<< endl;
                fout <<  "      <id " << q.id << "</id>" << endl;
                fout <<  "    </event>" << endl;
                fout <<  "  </events>" << endl;
                fout <<  "</earthquake>"   << endl;
            }
            fout.close();
        }
};
//menu to get users choice
int get_choice()
{
    int choice;

    cout << "[1] Comma seperated file \n [2] Create XML file" <<endl;
    cin >> choice;

    return choice;
};


int main ()
{

    Earthquake *eq;
    char filename [100];

    switch(get_choice())
    {
        case 1:
            eq = new CommaStream();
            strcpy(filename,"earthquake.csv");
            break;

        case 2:
            eq = new XMLStream();
            strcpy(filename, "earthquake.xml");
            break;
    }

    eq->load("earthquake.txt");
    eq->save(filename);

    delete eq;

    return 0;
}







