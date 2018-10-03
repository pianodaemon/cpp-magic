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
        /* still needed implementation*/
        return 0;
    };

    this->m_mutex.lock();
    int idx = req_next();
    this->m_slots[idx] = t;
    this->m_mutex.unlock();
    return idx;
}
