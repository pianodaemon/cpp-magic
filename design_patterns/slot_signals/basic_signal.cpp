#ifndef __SLOT_MANAGER_H__
#define __SLOT_MANAGER_H__
 
#include "basic_signal.h"
 
template <typename... T>
class slot_manager : public basic_signal<T...>
{
    public:
 
        slot_manager() { }
 
        template <typename F, typename... A>
 
        unsigned short conn_member(F&& f, A&& ... a) 
        {
            this->m_slots.insert({ ++this->m_slot_id, std::bind(f, a...) });
            return this->m_slot_id;
        }
 
    private:
 
         
};
 
 
 
#endif /* __SLOT_MANAGER_H__ */
