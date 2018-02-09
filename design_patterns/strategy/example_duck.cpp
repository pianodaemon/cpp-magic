#include <iostream>
#include <fstream>
#include <string>
#define NONE 0
 
using namespace std;
 
 
class DuckBehaviour
{
    public:
 
        virtual int fly() = NONE ; 
        virtual int cuack() = NONE ;
 
};
 
class RedDuckBehaviour : public DuckBehaviour
{
    public:
 
        int fly()
        {
            cout << " En zig zag" << endl;
            return 0;
        }
 
        int cuack()
        {
            cout << "Cuack Cuack!" << endl;
            return 0;
        }
 
};
 
 
 
class BlueDuckBehaviour : public DuckBehaviour
{
    public:
 
        int fly()
        {
            cout << "En circulos" << endl;
            return 0;
        }
 
        int cuack()
        {
            cout << "Bomba Bomba" << endl;
            return 0;
        }
 
};
 
class GreenDuckBehaviour : public DuckBehaviour
{
    public:
 
        int fly()
        {
            cout << "En linea recta" << endl;
            return 0;
        }
 
        int cuack()
        {
            cout << "Hay caramba cuakc!" << endl;
            return 0;
        }
 
};
 
class YellowDuckBehaviour : public DuckBehaviour
{
    public:
 
        int fly()
        {
            cout << "En Picada" << endl;
            return 0;
        }
 
        int cuack()
        {
            cout << "Pio Pio!" << endl;
            return 0;
        }
 
};
 
 
 
class DuckLoader
{
    public:
 
        DuckLoader();
 
        int runAs( int strategy );
 
        enum InternalMachineState
        {
            nonBehaviour,
            establishedBehaviour
        };
 
        int performFly();
        int performCuack();
         
 
    private:
        InternalMachineState currentState;
        void setDuckBehaviour(DuckBehaviour *selectedBehaviour);
        DuckBehaviour *selectedBehaviour;
         
         
};
 
 
DuckLoader::DuckLoader()
{
    this->currentState = nonBehaviour;
}
 
 
void  DuckLoader::setDuckBehaviour(DuckBehaviour *selectedBehaviour)
{
    this->selectedBehaviour = selectedBehaviour; 
}
 
int DuckLoader::performFly()
{
    return this->selectedBehaviour->fly();
}
 
int DuckLoader::performCuack()
{
    return this->selectedBehaviour->cuack();
}
 
 
int DuckLoader::runAs(int strategy)
{
 
    int returnCode = -1;
 
    DuckBehaviour *flavor = NONE ;
 
    if (  this->currentState  == establishedBehaviour  )
    {
        delete this->selectedBehaviour;
        this->currentState  = nonBehaviour;
    }
 
    if (  this->currentState == nonBehaviour  )
    {
 
        switch( strategy )
        {
            case 1:
            {
                flavor = new RedDuckBehaviour;
                break;
            }
            case 2:
            {
                flavor = new BlueDuckBehaviour;
                break;
            }
            case 3:
            {
                flavor = new GreenDuckBehaviour;
                break;
            }
            case 4:
            {
                flavor = new YellowDuckBehaviour;
                break;
            }
            default: /* never reached */
            {
                break;
            }
        }
 
        if ( flavor != NONE )
        {
            this->setDuckBehaviour( flavor );
            this->currentState  = establishedBehaviour;
        }
 
    }
 
    return returnCode;
 
}
 
 
 
 
int main()
{
 
 
    DuckLoader *loader = new DuckLoader;
 
     
    loader->runAs( 1 );
 
    loader->performCuack();
 
    loader->runAs( 2 );
 
    loader->performCuack();
 
    loader->runAs( 3 );
 
    loader->performCuack();
 
    loader->runAs( 4 );
 
    return loader->performCuack();
 
}
