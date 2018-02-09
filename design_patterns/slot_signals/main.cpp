#include "slot_manager.h"
#include <iostream>
#include <string>
 
int main(int argc, char* argv[])
{
    slot_manager<std::string, int> sm;
 
    sm.conn([](std::string arg1, int arg2) {
        std::cout << arg1 << " " << arg2 << std::endl;
    });
 
    sm.trigger("The answer:", 42);
 
 
    sm.disconn_all();
 
    return 0;
}
