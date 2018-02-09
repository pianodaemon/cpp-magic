#ifndef __GEN_ITEM__HPP__
#define	__GEN_ITEM__HPP__

namespace j4nusx
{
    class gen_item
    {
        public:
        
            virtual void state_a() = 0;
            virtual void state_b() = 0;
            virtual void state_c() = 0;
        
            virtual ~gen_item(){ /* nothing here yet! */ }
    };

}

#endif	/* __GEN_ITEM__HPP__ */
