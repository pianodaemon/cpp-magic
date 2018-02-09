#include "msfactory.hpp"
#include "gen_item.hpp"
#include <cstdint>
#include <memory>

/* The following example denotes how to implement a machine state
   as per the flavor requested by make method of a factory object */

int main(int argc, char** argv){

    using namespace j4nusx;
    msfactory<gen_item, uint8_t> fact;

    auto run_machine_state = [&](const uint8_t& m)->void
    {
        std::unique_ptr<gen_item> ms (fact.make(m));
        ms->state_a();
        ms->state_b();
        ms->state_c();
    };

    run_machine_state(MS_BLUE);
    run_machine_state(MS_RED);
 
    return 0;
}
