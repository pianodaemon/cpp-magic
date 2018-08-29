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
Misc::Pool<T>::place_at( const int index, T* t )
{
    pthread_mutex_lock( &this->m_mutex );
    this->m_slots[index] = t;
    pthread_mutex_unlock( &this->m_mutex );
}

template< typename T >
void
Misc::Pool<T>::destroy_at( const int index )
{
    this->place_at( index, nullptr );
}

template< typename T >
T*
Misc::Pool<T>::fetch_from( const int index )
{
    pthread_mutex_lock( &this->m_mutex );
    auto t = this->m_slots[index];
    pthread_mutex_unlock( &this->m_mutex );
    return t;
}
