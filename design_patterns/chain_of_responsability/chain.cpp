#include<stdlib.h>
#include <iostream>
#include <vector>
#include <ctime>
using namespace std;
 
class Base
{
    Base *next; 
  public:
    Base()
    {
        next = 0;
    }
    void setNext(Base *n)
    {
        next = n;
    }
    void add(Base *n)
    {
        if (next)
          next->add(n);
        else
          next = n;
    }
 
    virtual void handle(int i)
    {
        next->handle(i);
    }
};
 
class HandlerA: public Base
{
  public:
     void handle(int i)
    {
        if (rand() % 3)
        {
            cout << "HA passsed " << i << "  ";
            Base::handle(i); 
        }
        else
          cout << "HA handled " << i << "  ";
    }
};
 
class HandlerB: public Base
{
  public:
     void handle(int i)
    {
        if (rand() % 3)
        {
            cout << "HB passsed " << i << "  ";
            Base::handle(i);
        }
        else
          cout << "HB handled " << i << "  ";
    }
};
 
class HandlerC: public Base
{
  public:
     void handle(int i)
    {
        if (rand() % 3)
        {
            cout << "HC passsed " << i << "  ";
            Base::handle(i);
        }
        else
          cout << "HC handled " << i << "  ";
    }
};
 
int main()
{
  srand(time(0));
  HandlerA root;
  HandlerB two;
  HandlerC thr;
  root.add(&two);
  root.add(&thr);
  thr.setNext(&root);
   
  /* who will finish the task 1000 ? */
  root.handle(1000);
 
  cout << endl;
   
}
