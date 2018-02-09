#include <cstdio>
#include <iostream>
 
using namespace std;
 
// 1. Add an connect(Visitor) method to the "element" hierarchy
class Element
{
  public:
    virtual void connect(class Visitor &v) = 0;
};
 
class FtpIO: public Element
{
  public:
    void connect(Visitor &v);
 
};
 
class HttpIO: public Element
{
  public:
    void connect(Visitor &v);
 
};
 
// 2. Create a "visitor" base class w/ a visit() method for every "element" type
class Visitor
{
  public:
    virtual void visit(FtpIO *e) = 0;
    virtual void visit(HttpIO *e) = 0;
};
 
void FtpIO::connect(Visitor &v)
{
  v.visit(this);
}
 
void HttpIO::connect(Visitor &v)
{
  v.visit(this);
}
 
// 3. Create a "visitor" derived class for each "operation" to do on "elements"
class EngineAlpha: public Visitor
{
    void visit(FtpIO *e)
    {
        cout << "Engine Alpha Specific code for connecting to ftp url" << '\n';
    }
    void visit(HttpIO *e)
    {
        cout << "Engine Alpha Specific code for connecting to http url" << '\n';
    }
};
 
class EngineBeta: public Visitor
{
    void visit(FtpIO *e)
    {
        cout << "Engine Beta Specific code for connecting to ftp url" << '\n';
    }
    void visit(HttpIO *e)
    {
        cout << "Engine Beta Specific code for connecting to http url" << '\n';
    }
};
 
int main()
{
  Element *list[] = 
  {
    new FtpIO(), new HttpIO()
  };
  EngineAlpha engA; 
  EngineBeta engB; 
 
  for (int i = 0; i < 2; i++) list[i]->connect(engA);
   
  for (int j = 0; j < 2; j++) list[j]->connect(engB);
 
  return 0;
}
