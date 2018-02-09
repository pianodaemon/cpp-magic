#include "red.hpp"
#include <iostream>

namespace j4nusx 
{

    red::red()
    {
        std::cout << "Constructor red" << std::endl;
    }

    red::~red()
    {
        std::cout << "Destructor red" << std::endl;
    }
        
    void red::state_a()
    {
        std::cout << "state a in red" << std::endl;
    }
    void red::state_b()
    {
        std::cout << "state b in red" << std::endl;
    }

    void red::state_c()
    {
        std::cout << "state c in red" << std::endl;
    }

}
