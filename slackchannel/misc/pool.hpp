#ifndef __MISC_POOL_HPP__
#define __MISC_POOL_HPP__

#include <inttypes.h>
#include <vector>
#include <cstdlib>
#include <boost/thread/mutex.hpp>

namespace Misc
{

template< typename T >
class Pool
{
    public:

        Pool( const int );
        virtual ~Pool();
        void place_at( const int index, T* t );
        int place_smart( T* t );
        void destroy_at( const int index );
        T* fetch_from( const int index );

    private:

        enum Regs
        {
            START_REG,
            LAST_REG,
            INCREMENT_REG,
            NEXT_REG,
            MAX_REG
        };

        std::vector< T* > m_slots;
        boost::mutex m_mutex;
        uint8_t m_registers[ Regs::MAX_REG ];
};

}

#endif /*  __MISC_POOL_HPP__ */
