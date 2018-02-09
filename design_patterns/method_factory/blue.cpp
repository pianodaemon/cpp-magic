#include "blue.hpp"
#include <iostream>

namespace j4nusx 
{

    blue::blue()
    {
        std::cout << "Constructor blue" << std::endl;
    }

    blue::~blue()
    {
        std::cout << "Destructor blue" << std::endl;
    }
        
    void blue::state_a()
    {
        std::cout << "state a in blue" << std::endl;
    }
    void blue::state_b()
    {
        std::cout << "state b in blue" << std::endl;
    }

    void blue::state_c()
    {
        std::cout << "state c in blue" << std::endl;
    }

}
