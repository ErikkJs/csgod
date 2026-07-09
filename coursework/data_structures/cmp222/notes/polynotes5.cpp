#include <iostream>

using namespace std;

class IQueryable{
    public:
       virtual void find()=0
       virtual void get_all()=0
};

class ISortable{
    public:
        virtual void sort() =0;
};

class IEnumerable{
    public: 
        virtual void get_first() =0;
        virtual void get_all()=0;

};

class Dictionary : public IQueryable, public IEnumerable , public ISortable{
    public:
        void find(){}
        void sort(){}
        void get_first(){}
        void get_all(){}      
};


int main()
{

Dictionary dict;


}

