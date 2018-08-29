#include "pool.hpp"


template< typename T >
Misc::Pool<T>::Pool( const int size ) : m_slots( size, nullptr )
{
    pthread_mutex_init( &this->m_mutex , NULL );
}

template< typename T >
Misc::Pool<T>::~Pool()
{
    pthread_mutex_destroy( &this->m_mutex );
}

template< typename T >
void
Misc::Pool<T>::place_at( const int idx, T* t )
{
    pthread_mutex_lock( &this->m_mutex );
    this->m_slots[idx] = t;
    pthread_mutex_unlock( &this->m_mutex );
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
    pthread_mutex_lock( &this->m_mutex );
    auto t = this->m_slots[idx];
    pthread_mutex_unlock( &this->m_mutex );
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

    pthread_mutex_lock( &this->m_mutex );
    auto idx = req_next();
    this->m_slots[idx] = t;
    pthread_mutex_unlock( &this->m_mutex );
    return idx;
}
