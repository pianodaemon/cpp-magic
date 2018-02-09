#ifndef __RED_HPP__
#define	__RED_HPP__

#include "gen_item.hpp"

namespace j4nusx 
{

    class red : public gen_item
    {
        public:
        
            red();
            virtual ~red();
        
            void state_a();
            void state_b();
            void state_c();

    };

}
#endif  /* __RED_HPP__ */
