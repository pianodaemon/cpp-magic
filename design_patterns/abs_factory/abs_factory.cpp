#include<iostream>
 
class Button {
public:
    virtual void paint() = 0;
    virtual ~Button() { }
};
  
class WinButton : public Button {
public:
    void paint() {
        std::cout << "I'm a WinButton" << std::endl; 
    }
};
  
class OSXButton : public Button {
public:
    void paint() {
        std::cout << "I'm an OSXButton" << std::endl;
    }
};
  
class FactoryObject {
public:
    virtual Button* createButton() = 0;
    virtual ~FactoryObject() { }
};
  
class WinFactory : public FactoryObject {
public:
    Button* createButton() {
        return new WinButton();
    }
  
    ~WinFactory() { }
};
  
class OSXFactory : public FactoryObject {
public:
    Button* createButton() {
        return new OSXButton();
    }
  
    ~OSXFactory() { }
};
 
 
class ClientApplication {
 
public:
 
    static FactoryObject* createOsSpecificFactory() {
 
        int sys;
        std::cout << "Enter OS type (0: Windows, 1: MacOS X): ";
        std::cin >> sys;
  
        if (sys == 0) {
            return new WinFactory();
        } else {
            return new OSXFactory();
        }
    }
  
    static int main(int &argc, char** &argv){
        /*Note that the Application has no idea
         what kind of FactoryObject it is given
         or even what kind of Button that factory creates*/
        FactoryObject* factory = createOsSpecificFactory();
        Button* button = factory->createButton();
        button->paint();
        delete button;
        delete factory;
 
        return 0;
    }
 
 
};
 
int main(int argc, char** argv){
 
    return ClientApplication::main(argc , argv);
 
}
