#include <iostream>
using namespace std;

class FilePager{
    private:
        string _lines[200];
        `

    public:
        void first()
        {
        }

        void next()
        {
        }
        void previous()
        {
        }
        void last()
        {

        }
        void fill(char *file, int lines)
        {
            string line;
            ifstream ifs(file);
            if(ifs.is_open())
                while(!ifs.eof())
                {
                    int i =0;
                    ifs>>line;
                    _lines[i] = line;

                }
            ifs.close;
        }
        void line_set(int lines)
            for (int i= 0; i < lines; i++)
            {
                for(int k =0; k<lines; k++)
                {

                



            }
        int main(string file,int lines)
        {




        }

