#include "pool.hpp"


template< typename T >
Misc::Pool<T>::Pool( const int size ) : m_slots( size, nullptr )
{

}

template< typename T >
Misc::Pool<T>::~Pool()
{

}

template< typename T >
void
Misc::Pool<T>::place_at( const int idx, T* t )
{
    boost::mutex::scoped_lock scoped_lock(this->m_mutex);

    if ( t == nullptr )
    {
        delete this->m_slots[idx];
    }

    this->m_slots[idx] = t;
}

template< typename T >
void
Misc::Pool<T>::destroy_at( const int idx )
{
    this->place_at( idx, nullptr );
}

template< typename T >
T*
Misc::Pool<T>::fetch_from( const int idx )
{
    this->m_mutex.lock();
    T* t = this->m_slots[idx];
    this->m_mutex.unlock();
    return t;
}

template< typename T >
int
Misc::Pool<T>::place_smart( T* t )
{
    auto req_next = [&]()->int
    {
        uint8_t i = this->m_registers[ Regs::NEXT_REG ];

        if ( ( this->m_slots[ i ] != nullptr ) &&
                ( i == this->m_registers[ Regs::LAST_REG ] ) )
        {
            // From the first shelf we shall start
            // the quest of an available one if
            // next one was ocuppied and the last one.
            i = this->m_registers[ Regs::START_REG ];
        }

        if ( this->m_slots[ i ] == nullptr )
        {
            //  When the shelf is available we shall return it
            //  before we shall set next_num variable up for
            //  later calls to current function.
            if ( index == this->m_registers[ Regs::LAST_REG ] )
            {
                this->m_registers[ Regs::NEXT_REG ] = this->m_registers[ Regs::START_REG ];
            }
            else
            {
                uint8_t next_one = i + this->m_registers[ Regs::INCREMENT_REG ];
                this->m_registers[ Regs::NEXT_REG ] = next_one;
            }

            return i;
        }

        // If you've reached this code block my brother, so...
        //   you might be in trouble soon. By the way you seem
        //   a lucky folk and perhaps you would find a free
        //   shelf by performing sequential search with awful
        //   linear time. Otherwise the matter is fucked :(

        /* still needed implementation*/

    };

    this->m_mutex.lock();
    int idx = req_next();
    this->m_slots[idx] = t;
    this->m_mutex.unlock();
    return idx;
}
