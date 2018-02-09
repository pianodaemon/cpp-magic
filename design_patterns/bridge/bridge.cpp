#include <iostream>
#include <string>
  
/* Implemented interface. */
class AbstractInterface
{
    public:
        virtual void someFunctionality() = 0;
};
  
/* Interface for internal implementation that Bridge uses. */
class ImplementationInterface
{
    public:
        virtual void anotherFunctionality() = 0;
};
  
/* The Bridge */
class Bridge : public AbstractInterface
{
    protected:
        ImplementationInterface* implementation;
     
    public:
        Bridge(ImplementationInterface* backend)
        {
            implementation = backend;
        }
};
  
/* Different special cases of the interface. */
  
class UseCase1 : public Bridge
{
    public:
        UseCase1(ImplementationInterface* backend)
          : Bridge(backend)
        {}
  
        void someFunctionality()
        {
            std::cout << "UseCase1 on ";
            implementation->anotherFunctionality();
        }
};
  
class UseCase2 : public Bridge
{
    public:
        UseCase2(ImplementationInterface* backend)
          : Bridge(backend)
        {}
  
        void someFunctionality()
        {
            std::cout << "UseCase2 on ";
            implementation->anotherFunctionality();
        }
};
  
/* Different background implementations. */
  
class Windows : public ImplementationInterface
{
    public:
        void anotherFunctionality()
        {
            std::cout << "Windows" << std::endl;
        }
};
  
class Linux : public ImplementationInterface
{
    public:
        void anotherFunctionality()
        {
            std::cout << "Linux" << std::endl;
        }
};
  
int main()
{
    AbstractInterface *useCase = 0;
    ImplementationInterface *osWindows = new Windows;
    ImplementationInterface *osLinux = new Linux;
  
  
    /* First case */
    useCase = new UseCase1(osWindows);
    useCase->someFunctionality();
  
    useCase = new UseCase1(osLinux);
    useCase->someFunctionality();
  
    /* Second case */
    useCase = new UseCase2(osWindows);
    useCase->someFunctionality();
  
    useCase = new UseCase2(osLinux);
    useCase->someFunctionality();
  
    return 0;
}
