#ifndef __BLUE_HPP__
#define	__BLUE_HPP__

#include "gen_item.hpp"

namespace j4nusx 
{

    class blue : public gen_item
    {
        public:
        
            blue();
            virtual ~blue();
        
            void state_a();
            void state_b();
            void state_c();

    };

}
#endif  /* __BLUE_HPP__ */
