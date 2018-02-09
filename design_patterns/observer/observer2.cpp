#include <cstdio>
#include <vector>
#include <string>
  
using namespace std;
  
class Subject;
  
class Observer
{
    public:
        virtual void update(Subject *s) = 0;
};
  
class Subject
{
    public:
        void attach( Observer *o )
        {
            observers.push_back( o );
        }
  
        void detach()
        {
            if ( observers.size() > 0 )
            {
                observers.pop_back();
            }
        }
  
    protected:
        void notify()
        {
            for ( int i = 0, size = observers.size(); i < size; ++i )
            {
                observers[i]->update( this );
            }
        }
     
        vector< Observer* > observers;
};
  
class Data: public Subject
{
    public:
        Data( string n )
        {
            name = n;
            data = 0;
        };
         
        void setData( int d )
        {
            data = d;
            notify();
        }
         
        int getData()
        {
            return data;
        }
  
        string getName()
        {
            return name;
        }
  
    private:
        string name;
        int data;
};
  
class HexViewer: public Observer
{
    public:
        void update( Subject *s )
        {
            Data *d = (Data *)s;
            printf( "HexViewer: Subject %s has data 0x%x\n", d->getName().c_str(), d->getData() );
        }
};
  
class DecimalViewer: public Observer
{
    public:
        void update( Subject *s )
        {
            Data *d = (Data *)s;
            printf( "DecimalViewer: Subject %s has data %d\n", d->getName().c_str(), d->getData() );
        }
};
  
  
int main()
{
    Data data1 = Data( "Data 1" );
    Data data2 = Data( "Data 2" );
    DecimalViewer view1 = DecimalViewer();
    HexViewer view2 = HexViewer();
    
    data1.attach( &view1 );
    data1.attach( &view2 );
    data2.attach( &view1 );
    data2.attach( &view2 );
  
    printf( "Setting Data 1 = 10\n" );
    data1.setData(10);
    printf( "Setting Data 2 = 15\n" );
    data2.setData(15);
    printf( "Setting Data 1 = 3\n" );
    data1.setData(3);
    printf( "Setting Data 2 = 5\n" );
    data2.setData(5);
    
    printf( "Detach HexViewer from data1 and data2.\n" );
    data1.detach();
    data2.detach();
    printf( "Setting Data 1 = 10\n" );
    data1.setData(10);
    printf( "Setting Data 2 = 15\n" );
    data2.setData(15);
    
    return 0;
}
