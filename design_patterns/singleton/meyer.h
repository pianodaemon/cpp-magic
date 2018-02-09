#ifndef __MEYER_SINGLETON_HPP__
#define __MEYER_SINGLETON_HPP__
 
template< class T >
class MeyerSingleton
{
    public:
 
        static T& getInstance(void)
        {
            static T instance;
            return instance;
        }
 
 
    private:
 
        MeyerSingleton();
        ~MeyerSingleton();
};
 
 
#endif /* __MEYER_SINGLETON_HPP__ */
