#include <iostream>
#include <fstream>

using namespace std;

class BaseFile{
    protected:
        string name;
        int size;
    public:
 void  set_name(string n){
  
      name = n;

  }

};
class ImageFile : public BaseFile {
    protected: 
        int height;
        int width;
       
        ImageFile(int h, int w)
        {
            height = h;
            width =w;
        }

};

class FramedImageFile : public ImageFile
{
    private:
       int  _frames;

    public:
       int height2;
      int width2;
        
        FramedImageFile(int f , int h, int w):
          _frames(f), ImageFile(h,w)
    {
    } 
   
      friend ostream & operator << ( ostream & show ,FramedImageFile & f)
        {
            show<< f.height << " X " <<f.width << " X " <<f._frames;

            return show;
        }

};


int main()
{
    int frames;
    int height;
    int width;
    string name;

    cout << "enter frames" << endl;
    cin>> frames;

    cout << "enter height" << endl;
    cin >> height;

    cout << "enter width" << endl;
    cin >> width;

    cout << "enter a name" << endl;
    cin >> name;


    FramedImageFile * f = new FramedImageFile(frames,height,width);

    f->set_name(name);
   
    cout << *f;
    delete f;

    return 0;
}

