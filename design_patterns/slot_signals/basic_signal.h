#ifndef __BASIC_SIGNAL_H__
#define __BASIC_SIGNAL_H__
 
#include <functional>
#include <unordered_map>
 
template <typename... A>
class basic_signal
{
    protected:
 
        std::unordered_map<int, std::function<void(A...)>> m_slots;
        unsigned short m_slot_id;
 
    public:
 
        basic_signal() : m_slot_id(0) {}
 
        unsigned short conn(std::function<void(A...)> const& s)
        {
            this->m_slots.insert(std::make_pair(++this->m_slot_id, s));
            return this->m_slot_id;
        }
 
        void disconn(unsigned short id)
        {
            this->m_slots.erase(id);
        }
 
        void disconn_all()
        {
            this->m_slots.clear();
        }
 
        void trigger(A... p)
        {
            for (auto iter : this->m_slots) iter.second(p...);
        }
};
 
#endif /* __BASIC_SIGNAL_H__ */
