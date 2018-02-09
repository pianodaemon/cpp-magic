#include <iostream>
#include <string>
 
using namespace std;
 
/* START ------------------ The Receiver class */
class Light {
 
   public:
        Light();
        void turnOn();
        void turnOff();
};
 
Light::Light(){
 
}
  
void Light::turnOn() {
    cout << "The light is on" << endl;
}
  
void Light::turnOff() {
    cout <<  "The light is off" << endl;
}
 
/* END ------------------ The Receiver class */
 
/* START --------------- The Command interface */
class Command {
 
    public:
        virtual void execute() = 0;
};
/* END ---------------- The Command interface */
 
/* START --------------- The Command for turning on the light - ConcreteCommand #1 */
 
class FlipUpCommand: public Command {
 
   private:
        Light theLight;
  
   public:
        FlipUpCommand(Light &light);
        void execute();
};
 
FlipUpCommand::FlipUpCommand(Light &light) {
    this->theLight = light;
}
  
void FlipUpCommand::execute(){
    this->theLight.turnOn();
}
 
 
/* END ----------------- The Command for turning on the light - ConcreteCommand #1 */
 
/* START ---------------  The Command for turning off the light - ConcreteCommand #2 */
 
class FlipDownCommand: public Command {
 
   private:
        Light theLight;
  
   public:
        FlipDownCommand(Light &light);
        void execute();
};
 
FlipDownCommand::FlipDownCommand(Light &light) {
    this->theLight = light;
}
  
void FlipDownCommand::execute(){
    this->theLight.turnOff();
}
 
 
/* END ---------------  The Command for turning off the light - ConcreteCommand #2 */
 
class Invoker
{
    private:
 
    public:
        Invoker();
        void executeCommand( Command *cmd );
};
 
Invoker::Invoker()
{
     
}
 
void Invoker::executeCommand( Command *cmd )
{
    cmd->execute();
}
 
 
int main()
{
    Light lamp;
    Invoker inv;
 
    Command *switchUp = new FlipUpCommand( lamp );
    Command *switchDown = new FlipDownCommand( lamp );
 
    inv.executeCommand(switchUp);
    inv.executeCommand(switchDown);
 
    delete switchUp;
    delete switchDown;
 
    return 0;
}
