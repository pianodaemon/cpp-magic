#ifndef __MS_FACTORY_HPP__
#define	__MS_FACTORY_HPP__

#include "gen_loader.hpp"
#include "blue.hpp"
#include "red.hpp"

namespace j4nusx
{
    const uint8_t MS_BLUE = 0x01;
    const uint8_t MS_RED = 0x02;

    template< typename W , typename K >
    class msfactory
    {
        public:
            msfactory()
            {
                #define __SUBSCRIBE(L, MS_ID, MS_CLASS) \
                    L.subscribe( MS_ID, new gen_inceptor<MS_CLASS, W>() )

                // This is the place where subscribing new
                // machine states to be incepted.
                __SUBSCRIBE( m_gl, MS_BLUE, blue );
                __SUBSCRIBE( m_gl, MS_RED, red );

                #undef __SUBSCRIBE
            }

            virtual ~msfactory(){ }
            
            W* make( const K& ms_id )
            {
                return m_gl.incept( ms_id );
            }

        private:
            gen_loader<W, K> m_gl;
    };
}

#endif  /* __MS_FACTORY_HPP__ */
