#ifndef __GEN_LOADER_HPP__
#define __GEN_LOADER_HPP__

#include <map>
#include <iostream>
#include <memory>

namespace j4nusx 
{
    template< typename W >
    class igen_inceptor
    {
        public:
            virtual W* incept() = 0;
            virtual ~igen_inceptor(){ std::cout << "Destroying inceptor instance" << std::endl; } 
    };

    template< typename T , typename W >
    class gen_inceptor : public igen_inceptor<W>
    {

        static_assert(
            std::is_base_of<W, T>::value,
            "T must be a descendant of W"
        );


        public:

            W* incept()
            {
                T* t_ptr = nullptr;

                try
                {
                    t_ptr = new T();
                }
                catch ( std::bad_alloc )
                {
                    std::cout << "Memory allocation failed for gen_inceptor Instance. Ending Application" << std::endl;
                }

                return t_ptr;
            }
    };

    template< typename W , typename K >
    class gen_loader
    {
        public:
            gen_loader() { }
            virtual ~gen_loader() { }

            void subscribe( K id, igen_inceptor<W>* ic_ptr )
            {
                this->m_map[ id ] = std::shared_ptr<igen_inceptor<W>>(ic_ptr);
            }

            W* incept( K id )
            {
                return this->m_map[ id ]->incept();
            }

            bool is_supported( K id )
            {
                // Be aware of it is taking linear time
                typename std::map< K , std::shared_ptr<igen_inceptor<W>> >::const_iterator it = this->m_map.find( id );
    
                if ( it != this->m_map.end() )
                {
                    // element was found !
                    return true;
                }
        
                return false;
            }

        private:
            std::map< K , std::shared_ptr< igen_inceptor<W> > > m_map;
    };

}

#endif /* __GEN_LOADER_HPP__ */
