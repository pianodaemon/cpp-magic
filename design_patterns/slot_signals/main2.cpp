#include "slot_manager.h"
#include <iostream>
#include <string>
 
class Button {
public:
    slot_manager<> on_click;
};
 
class Message {
public:
    void display() const {
        std::cout << "Hello World!" << std::endl;
    }
};
 
int main(int argc, char* argv[])
{
    Button  button;
    Message message;
 
    button.on_click.conn_member(&Message::display, message);
    button.on_click.trigger();
 
    return 0;
}
